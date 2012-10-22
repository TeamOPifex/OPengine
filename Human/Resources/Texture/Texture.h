#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include "Core\include\Types.h"

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

class Texture {
public:
	~Texture() {
		free(_buffer);
	}
	ui32 Width() { return _width; }
	ui32 Height() { return _height; }
	ui32 LinearSize() { return _linearSize; }
	ui32 MipMapCount() { return _mipMapCount; }
	ui32 Blocksize() { return _blocksize; }
	ui32 FourCC() { return _fourCC; }
	ui8* Buffer() { return _buffer; }
	ui32 Format() { return _format; }
protected:
	ui32 _width;
	ui32 _height;
	ui32 _linearSize;
	ui32 _mipMapCount;
	ui32 _blocksize;
	ui32 _fourCC;
	ui32 _format;
	ui8* _buffer;
	ui32 bufsize;
};

class TextureDDS : public Texture {
protected:
	ui8* tmp;
public:
	TextureDDS(const char * path){
		unsigned char header[124];
		FILE *fp;

		fp = fopen(path, "rb");
		if(fp == NULL)
			return;

		i8 filecode[4];
		fread(filecode, 1, 4, fp);
		if(strncmp(filecode, "DDS ", 4) != 0){
			fclose(fp);
			return;
		}

		fread(&header, 124, 1, fp);
		
		_height      = *(ui32*)&(header[8 ]);
		_width	     = *(ui32*)&(header[12]);
		_linearSize	 = *(ui32*)&(header[16]);
		_mipMapCount = *(ui32*)&(header[24]);
		_fourCC      = *(ui32*)&(header[80]);


		bufsize = _mipMapCount > 1 ? _linearSize * 2 : _linearSize;
		_buffer = (ui8*)malloc(bufsize * sizeof(ui8));
		tmp = _buffer;
		fread(_buffer, 1, bufsize, fp);
		fclose(fp);

		ui32 components = (_fourCC == FOURCC_DXT1) ? 3 : 4;
		switch(_fourCC){
		case FOURCC_DXT1:
			_format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			break;
		case FOURCC_DXT3:
			_format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			break;
		case FOURCC_DXT5:
			_format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
		default:
			free(_buffer);
			return;
		}

		_blocksize = (_format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	}
};

class GLTexture {
private:
	ui32 _textureID;
public:
	GLTexture(Texture* texture){
		glGenTextures(1, &_textureID);

		glBindTexture(GL_TEXTURE_2D, _textureID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		ui32 offset = 0;

		ui32 width, height;
		width = texture->Width();
		height = texture->Height();
		ui8* buffer = texture->Buffer();

		for(ui32 level = 0; level < texture->MipMapCount() && (texture->Width() || texture->Height()); ++level)
		{
			ui32 size = ((texture->Width() + 3) / 4) * ((texture->Height() + 3) / 4) * texture->Blocksize();
			glCompressedTexImage2D(GL_TEXTURE_2D, level, texture->Format(), width, height, 0, size, buffer + offset);

			offset += size;
			width /= 2;
			height /= 2;
		}
	}

	ui32 textureID() { return _textureID; }
};
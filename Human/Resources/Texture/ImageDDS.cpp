#include "ImageDDS.h"

#include <string.h>
#include "./Core/include/Log.h"
#include "./Data/include/OPfile.h"

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

#if defined(OPIFEX_OPENGL_ES_2)
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
	#define OP_GL_COMPRESSED_RGBA_S3TC_DXT1_EXT GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
	#define OP_GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2 // This is for Ouya, extension is supported in Tegra 3
#else
	#include <GL/glew.h>
	#include <GL/glfw.h>
	#define OP_GL_COMPRESSED_RGBA_S3TC_DXT1_EXT GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
	#define OP_GL_COMPRESSED_RGBA_S3TC_DXT3_EXT GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
	#define OP_GL_COMPRESSED_RGBA_S3TC_DXT5_EXT GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
#endif


ImageDDS::ImageDDS(FILE* fp){
	unsigned char header[124];
		if(fp == NULL){
			OPLog("TextureDDS::Error 1 - There was a problem with the FILE pointer");
			return;
		}

		char filecode[4];
		fread(filecode, 1, 4, fp);
		if(strncmp(filecode, "DDS ", 4) != 0){
			OPLog("This is not a DDS. Closing.");
			fclose(fp);
			return;
		}
		
		fread(&header, 124, 1, fp);
		
		_height      = *(ui32*)&(header[8 ]);
		_width	     = *(ui32*)&(header[12]);
		_linearSize	 = *(ui32*)&(header[16]);
		_mipMapCount = *(ui32*)&(header[24]);
		_fourCC      = *(ui32*)&(header[80]);
		_compressed = true;
		
		bufsize = _mipMapCount > 1 ? _linearSize * 2 : _linearSize;
		_buffer = (ui8*)malloc(bufsize * sizeof(ui8));
		tmp = _buffer;
		fread(_buffer, 1, bufsize, fp);
		fclose(fp);

		ui32 components = (_fourCC == FOURCC_DXT1) ? 3 : 4;
		switch(_fourCC){
		case FOURCC_DXT1:
			_format = OP_GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			OPLog("This is a DXT1 File!");
			break;
		case FOURCC_DXT3:
			// This is only supported in OpenGL ES 2.0 because Tegra 3 is our 
			// lowest common denominator, which does have support for this compress DXT
			_format = OP_GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			break;
#if !defined(OPIFEX_OPENGL_ES_2)
			// OpenGL ES 2.0 doesn't support DXT5 for any device currently (Tegra 4 might)
		case FOURCC_DXT5:
			_format = OP_GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;		
#endif
		default:			
			OPLog("Could not determine the file format or file format was not supported!");
			free(_buffer);
			return;
		}

		_blocksize = (_format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
}

ImageDDS* ImageDDS::FromFile(const char* file) {
	FileInformation fileInfo = OPreadFileInformation(file);
	ImageDDS* tex = new ImageDDS(fileInfo.file);
	return tex;
}

Texture2D* ImageDDS::TextureFromFile(const char* file) {
	ImageDDS* imageData = ImageDDS::FromFile(file);
	Texture2D* texture = new Texture2D(imageData);
	delete imageData;
	return texture;
}
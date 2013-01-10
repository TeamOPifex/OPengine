#include "TextureDDS.h"

#include <string.h>
#include "./Core/include/Log.h"

#if defined(OPIFEX_OPENGL_ES_2)
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#else
	#include <GL/glew.h>
	#include <GL/glfw.h>
#endif

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

TextureDDS::TextureDDS(FILE* fp){
	unsigned char header[124];
		if(fp == NULL){
			OPLog("TextureDDS::Error 1 - There was a problem with the FILE pointer");
			return;
		}

		char filecode[4];
		fread(filecode, 1, 4, fp);
		OPLog("File Code: ");
		OPLog(filecode);
		if(strncmp(filecode, "DDS ", 4) != 0){
			OPLog("This is not a DDS. Closing.");
			fclose(fp);
			return;
		}
		OPLog("This is a DDS File!");
		
		fread(&header, 124, 1, fp);
		
		_height      = *(ui32*)&(header[8 ]);
		_width	     = *(ui32*)&(header[12]);
		_linearSize	 = *(ui32*)&(header[16]);
		_mipMapCount = *(ui32*)&(header[24]);
		_fourCC      = *(ui32*)&(header[80]);
		
		OPLog_i32(_height);
		OPLog_i32(_width);

		bufsize = _mipMapCount > 1 ? _linearSize * 2 : _linearSize;
		_buffer = (ui8*)malloc(bufsize * sizeof(ui8));
		tmp = _buffer;
		fread(_buffer, 1, bufsize, fp);
		fclose(fp);

		ui32 components = (_fourCC == FOURCC_DXT1) ? 3 : 4;
		switch(_fourCC){
		case FOURCC_DXT1:
			_format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			OPLog("This is a DXT1 File!");
			break;
#if !defined(OPIFEX_OPENGL_ES_2)
		case FOURCC_DXT3:
			_format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			break;
		case FOURCC_DXT5:
			_format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
#endif
		default:			
			OPLog("Could not determine the file format!");
			free(_buffer);
			return;
		}

		_blocksize = (_format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
			OPLog("We're Golden!");
}

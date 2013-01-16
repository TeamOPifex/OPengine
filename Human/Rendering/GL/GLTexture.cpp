#include "GLTexture.h"
#include "GLUtility.h"

#if defined(OPIFEX_ANDROID)
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#else
	#include <GL/glew.h>
	#include <GL/glfw.h>
#endif

GLTexture::GLTexture(Texture* texture){
	glGenTextures(1, &_textureID);
	GLUtility::CheckError("GLTexture::Error 1");

	glBindTexture(GL_TEXTURE_2D, _textureID);
	GLUtility::CheckError("GLTexture::Error 2");

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	GLUtility::CheckError("GLTexture::Error 3");

	ui32 offset = 0;

	ui32 width, height;
	width = texture->Width();
	height = texture->Height();
	ui8* buffer = texture->Buffer();

	for(ui32 level = 0; level < texture->MipMapCount() && (width || height); ++level)
	{
		ui32 size = ((width + 3) / 4) * ((height + 3) / 4) * texture->Blocksize();
		glCompressedTexImage2D(GL_TEXTURE_2D, level, texture->Format(), width, height, 0, size, buffer + offset);
			
		if(GLUtility::CheckError("GLTexture::Error 1"))
			return;

		offset += size;
		width /= 2;
		height /= 2;
	}
}

void GLTexture::bind(ui32 loc, int texture){
	glActiveTexture(GL_TEXTURE0 + texture);
	glBindTexture(GL_TEXTURE_2D, _textureID);
	glUniform1i(loc, texture);
}
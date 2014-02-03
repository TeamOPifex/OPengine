#include "./Human/include/Resources/Texture/Texture2D.h"
#include "./Human/include/Utilities/Errors.h"

#if defined(OPIFEX_ANDROID)
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif
#include "./Core/include/Log.h"


Texture2D::Texture2D(ImagePtr texture){
	glGenTextures(1, &m_handle);
	CheckError("Texture2D::Error 1");

	glBindTexture(GL_TEXTURE_2D, m_handle);
	CheckError("Texture2D::Error 2");

	if(texture->Compressed()) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		CheckError("Texture2D::Error 3");
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //GL_NEAREST = no smoothing
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	ui32 offset = 0;

	ui32 width, height;
	width = texture->Width();
	height = texture->Height();
	ui8* buffer = texture->Buffer();


	for(ui32 level = 0; level < texture->MipMapCount() && (width || height); ++level)
	{
		ui32 size = ((width + 3) / 4) * ((height + 3) / 4) * texture->Blocksize();

		if(texture->Compressed()) {
			glCompressedTexImage2D(GL_TEXTURE_2D, level, texture->Format(), width, height, 0, size, buffer + offset);
		} else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
		}
		if(CheckError("Texture2D::Error 4"))
			return;

		offset += size;
		width /= 2;
		height /= 2;
	}
}

void Texture2D::Bind(ui32 loc, int slot){

	glActiveTexture(GL_TEXTURE0 + slot);
	CheckError("Texture2D::Error 5 - Failed to bind texture");
	glBindTexture(GL_TEXTURE_2D, m_handle);
	CheckError("Texture2D::Error 6 - Failed to bind texture");
	glUniform1i(loc, slot);
	if(CheckError("Texture2D::Error 7 - Failed to bind texture")){

	}
}
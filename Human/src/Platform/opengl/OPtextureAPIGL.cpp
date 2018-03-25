
#include "./Human/include/Platform/opengl/OPtextureAPIGL.h"

#ifdef OPIFEX_OPENGL

#include "./Human/include/Platform/opengl/OPcommonGL.h"
#include "./Human/include/Rendering/OPtexture.h"
#include "./Core/include/OPmemory.h"



OPtexture* OPtextureGLInit(OPtexture* texture, OPtextureDesc textureDesc, const void* pixelData) {
	OPtextureGL* internalPtr = (OPtextureGL*)OPalloc(sizeof(OPtextureGL));;
	texture->internalPtr = internalPtr;
	texture->textureDesc = textureDesc;

	OPGLFN(glGenTextures(1, &internalPtr->Handle));
	OPGLFN(glActiveTexture(GL_TEXTURE0 + 0));


	ui32 textureInternalFormat = OPtextureFormatToGL(textureDesc.internalFormat);
	ui32 textureFormat = OPtextureFormatToGL(textureDesc.format);
	ui32 textureMinFilter = OPtextureFilterToGL(textureDesc.minfilter);
	ui32 textureMagFilter = OPtextureFilterToGL(textureDesc.magfilter);
	ui32 textureWrap = OPtextureWrapToGL(textureDesc.wrap);
	ui32 textureType = OPtextureTypeToGL(textureDesc.textureType);
	
	if (textureDesc.multisampled) {
		OPGLFN(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, internalPtr->Handle));

		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, textureInternalFormat, textureDesc.width, textureDesc.height, true);

		//OPGLFN(glTexImage2D(GL_TEXTURE_2D_MULTISAMPLE, 0, textureInternalFormat, textureDesc.width, textureDesc.height, 0, textureFormat, textureType, pixelData));
		
		//OPGLFN(glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, textureWrap));
		//OPGLFN(glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, textureWrap));
		//OPGLFN(glTexImage2D(GL_TEXTURE_2D_MULTISAMPLE, 0, textureInternalFormat, textureDesc.width, textureDesc.height, 0, textureFormat, textureType, pixelData));

		//float aniso = 0.0f;
		//glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
		//glTexParameterf(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);

		//OPGLFN(glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, textureMinFilter));
		//OPGLFN(glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, textureMagFilter));

		if (textureDesc.mipmap) {
			//OPGLFN(glGenerateMipmap(GL_TEXTURE_2D_MULTISAMPLE));
		}
	}
	else {
		OPGLFN(glBindTexture(GL_TEXTURE_2D, internalPtr->Handle));
		OPGLFN(glTexImage2D(GL_TEXTURE_2D, 0, textureInternalFormat, textureDesc.width, textureDesc.height, 0, textureFormat, textureType, pixelData));
		
		//float aniso = 0.0f;
 		//	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);

		OPGLFN(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrap));
		OPGLFN(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrap));

		OPGLFN(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureMinFilter));
		OPGLFN(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureMagFilter));

		if (textureDesc.mipmap) {
			OPGLFN(glGenerateMipmap(GL_TEXTURE_2D));
		}

	}

	return texture;
}

OPtexture* OPvertexBufferGLCreate(OPtextureDesc textureDesc, const void* pixelData) {
	OPtexture* texture = (OPtexture*)OPalloc(sizeof(OPtexture));
	return OPtextureGLInit(texture, textureDesc, pixelData);
}

void OPtextureGLSetData(OPtexture* ptr, const void* pixelData) {
	OPtextureGL* texture = (OPtextureGL*)ptr->internalPtr;
	if (ptr->textureDesc.multisampled) {
		OPGLFN(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture->Handle));
		OPGLFN(glTexSubImage2D(GL_TEXTURE_2D_MULTISAMPLE, 0, 0, 0, ptr->textureDesc.width, ptr->textureDesc.height, OPtextureFormatToGL(ptr->textureDesc.format), OPtextureTypeToGL(ptr->textureDesc.textureType), pixelData));
	}
	else {
		OPGLFN(glBindTexture(GL_TEXTURE_2D, texture->Handle));
		OPGLFN(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, ptr->textureDesc.width, ptr->textureDesc.height, OPtextureFormatToGL(ptr->textureDesc.format), OPtextureTypeToGL(ptr->textureDesc.textureType), pixelData));
	}
}

void OPtextureGLDestroy(OPtexture* ptr) {
	OPtextureGL* textureGL = (OPtextureGL*)ptr->internalPtr;
	OPGLFN(glDeleteTextures(1, &textureGL->Handle));
	OPfree(textureGL);
	ptr->internalPtr = NULL;
}

void OPtextureGLBind(OPtexture* ptr, ui32 slot) {
	if (ptr == NULL) return;

	OPtextureGL* texture = (OPtextureGL*)ptr->internalPtr;
	OPGLFN(glActiveTexture(GL_TEXTURE0 + slot));
	if (ptr->textureDesc.multisampled) {
		OPGLFN(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture->Handle));
	}
	else {
		OPGLFN(glBindTexture(GL_TEXTURE_2D, texture->Handle));
	}
}

void OPtextureGLUnbind(OPtexture* ptr, ui32 slot) {
	OPtextureGL* texture = (OPtextureGL*)ptr->internalPtr;
	OPGLFN(glActiveTexture(GL_TEXTURE0 + slot));
	if (ptr->textureDesc.multisampled) {
		OPGLFN(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
	}
	else {
		OPGLFN(glBindTexture(GL_TEXTURE_2D, 0));
	}
}

void OPtextureGLSetMinFilter(OPtexture* ptr, OPtextureFilter::Enum filter) {
	OPtextureGLBind(ptr, 0);
	ui32 filterGL = OPtextureFilterToGL(filter);
	OPGLFN(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterGL));
}

void OPtextureGLSetMagFilter(OPtexture* ptr, OPtextureFilter::Enum filter) {
	OPtextureGLBind(ptr, 0);
	ui32 filterGL = OPtextureFilterToGL(filter);
	OPGLFN(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterGL));
}

void OPtextureAPIGLInit(OPtextureAPI* texture) {
	texture->_Init = OPtextureGLInit;
	texture->_Create = OPvertexBufferGLCreate;
	texture->SetData = OPtextureGLSetData;
	texture->Bind = OPtextureGLBind;
	texture->Unbind = OPtextureGLUnbind;
	texture->Destroy = OPtextureGLDestroy;
	texture->SetMinFilter = OPtextureGLSetMinFilter;
	texture->SetMagFilter = OPtextureGLSetMagFilter;
}

#endif
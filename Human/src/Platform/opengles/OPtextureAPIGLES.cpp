
#include <External/GLES2/GLES2/gl2.h>
#include "./Human/include/Platform/opengles/OPtextureAPIGLES.h"

#ifdef OPIFEX_OPENGL_ES_2
#include "./Human/include/Platform/opengles/OPcommonGLES.h"
#include "./Human/include/Rendering/OPtexture.h"
#include "./Core/include/OPmemory.h"


ui32 OPtextureWrapToGLES(OPtextureWrap::Enum textureWrap) {
	switch (textureWrap) {
		//case OPtextureWrap::CLAMP: return GL_CLAMP;
		//case OPtextureWrap::CLAMP_TO_BORDER: return GL_CLAMP_TO_BORDER;
		case OPtextureWrap::CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
		case OPtextureWrap::REPEAT: return GL_REPEAT;
		case OPtextureWrap::MIRRORED_REPEAT: return GL_MIRRORED_REPEAT;
	}
	return 0;
}

ui32 OPtextureFormatToGLES(OPtextureFormat::Enum textureFormat) {
	switch (textureFormat)
	{
	case OPtextureFormat::RGBA: return GL_RGBA;
	//case OPtextureFormat::RGBA8: return GL_RGBA8;
	case OPtextureFormat::RGB: return GL_RGB;
	//case OPtextureFormat::BGRA: return GL_BGRA;
	//case OPtextureFormat::BGR: return GL_BGR;
	case OPtextureFormat::LUMINANCE: return GL_LUMINANCE;
	case OPtextureFormat::LUMINANCE_ALPHA: return GL_LUMINANCE_ALPHA;
	//case OPtextureFormat::RGB16F: return GL_RGB16F;
	//case OPtextureFormat::RGB32F: return GL_RGB32F;
	//case OPtextureFormat::RGBA16F: return GL_RGBA16F;
	//case OPtextureFormat::RGBA32F: return GL_RGBA32F;
	case OPtextureFormat::DEPTH: return GL_DEPTH_COMPONENT;
	case OPtextureFormat::DEPTH16F: return GL_DEPTH_COMPONENT16;
	//case OPtextureFormat::DEPTH32F: return GL_DEPTH_COMPONENT32F;
	//case OPtextureFormat::RED: return GL_RED;
	}
	return 0;
}

ui32 OPtextureFilterToGLES(OPtextureFilter::Enum textureFilter) {
	switch (textureFilter) {
	case OPtextureFilter::LINEAR: return GL_LINEAR;
	case OPtextureFilter::NEAREST: return GL_NEAREST;
	case OPtextureFilter::LINEAR_MIPMAP: return GL_NEAREST_MIPMAP_LINEAR;
	}
	return 0;
}

ui32 OPtextureTypeToGLES(OPtextureType::Enum textureType) {
	switch (textureType) {
	case OPtextureType::BYTE: return GL_UNSIGNED_BYTE;
	case OPtextureType::FLOAT: return GL_FLOAT;
	}
	return 0;
}

OPtexture* OPtextureGLESInit(OPtexture* texture, OPtextureDesc textureDesc, const void* pixelData) {
	OPtextureGLES* internalPtr = (OPtextureGLES*)OPalloc(sizeof(OPtextureGLES));;
	texture->internalPtr = internalPtr;
	texture->textureDesc = textureDesc;

	OPGLFN(glGenTextures(1, &internalPtr->Handle));
	OPGLFN(glActiveTexture(GL_TEXTURE0 + 0));


	ui32 textureInternalFormat = OPtextureFormatToGLES(textureDesc.internalFormat);
	ui32 textureFormat = OPtextureFormatToGLES(textureDesc.format);
	ui32 textureMinFilter = OPtextureFilterToGLES(textureDesc.minfilter);
	ui32 textureMagFilter = OPtextureFilterToGLES(textureDesc.magfilter);
	ui32 textureWrap = OPtextureWrapToGLES(textureDesc.wrap);
	ui32 textureType = OPtextureTypeToGLES(textureDesc.textureType);

	if (textureDesc.multisampled) {
		//OPGLFN(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, internalPtr->Handle));

		//glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, textureInternalFormat, textureDesc.width, textureDesc.height, true);

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

		OPGLFN(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrap));
		OPGLFN(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrap));
		OPGLFN(glTexImage2D(GL_TEXTURE_2D, 0, textureInternalFormat, textureDesc.width, textureDesc.height, 0, textureFormat, textureType, pixelData));
		
		float aniso = 0.0f;
 		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);

		OPGLFN(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureMinFilter));
		OPGLFN(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureMagFilter));

		if (textureDesc.mipmap) {
			OPGLFN(glGenerateMipmap(GL_TEXTURE_2D));
		}

	}

	return texture;
}

OPtexture* OPvertexBufferGLESCreate(OPtextureDesc textureDesc, const void* pixelData) {
	OPtexture* texture = (OPtexture*)OPalloc(sizeof(OPtexture));
	return OPtextureGLESInit(texture, textureDesc, pixelData);
}

void OPtextureGLESSetData(OPtexture* ptr, const void* pixelData) {
	OPtextureGLES* texture = (OPtextureGLES*)ptr->internalPtr;
	if (ptr->textureDesc.multisampled) {
		//OPGLFN(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture->Handle));
		//OPGLFN(glTexSubImage2D(GL_TEXTURE_2D_MULTISAMPLE, 0, 0, 0, ptr->textureDesc.width, ptr->textureDesc.height, OPtextureFormatToGLES(ptr->textureDesc.format), GL_UNSIGNED_BYTE, pixelData));
	}
	else {
		OPGLFN(glBindTexture(GL_TEXTURE_2D, texture->Handle));
		OPGLFN(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, ptr->textureDesc.width, ptr->textureDesc.height, OPtextureFormatToGLES(ptr->textureDesc.format), GL_UNSIGNED_BYTE, pixelData));
	}
}

void OPtextureGLESDestroy(OPtexture* ptr) {
	OPtextureGLES* textureGL = (OPtextureGLES*)ptr->internalPtr;
	OPGLFN(glDeleteTextures(1, &textureGL->Handle));
	OPfree(textureGL);
	ptr->internalPtr = NULL;
}

void OPtextureGLESBind(OPtexture* ptr, ui32 slot) {
	if (ptr == NULL) return;

	OPtextureGLES* texture = (OPtextureGLES*)ptr->internalPtr;
	OPGLFN(glActiveTexture(GL_TEXTURE0 + slot));
	if (ptr->textureDesc.multisampled) {
		//OPGLFN(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture->Handle));
	}
	else {
		OPGLFN(glBindTexture(GL_TEXTURE_2D, texture->Handle));
	}
}

void OPtextureGLESUnbind(OPtexture* ptr, ui32 slot) {
	OPtextureGLES* texture = (OPtextureGLES*)ptr->internalPtr;
	OPGLFN(glActiveTexture(GL_TEXTURE0 + slot));
	if (ptr->textureDesc.multisampled) {
		//OPGLFN(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
	}
	else {
		OPGLFN(glBindTexture(GL_TEXTURE_2D, 0));
	}
}

void OPtextureAPIGLESInit(OPtextureAPI* texture) {
	texture->_Init = OPtextureGLESInit;
	texture->_Create = OPvertexBufferGLESCreate;
	texture->SetData = OPtextureGLESSetData;
	texture->Bind = OPtextureGLESBind;
	texture->Unbind = OPtextureGLESUnbind;
	texture->Destroy = OPtextureGLESDestroy;
}

#endif
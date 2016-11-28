#include "./Human/include/Platform/opengl/OPtextureAPIGL.h"
#include "./Human/include/Platform/opengl/OPcommonGL.h"
#include "./Human/include/Rendering/OPtexture.h"
#include "./Core/include/OPmemory.h"


ui32 OPtextureWrapToGL(OPtextureWrap textureWrap) {
	switch (textureWrap) {
		case OPtextureWrap::CLAMP: return GL_CLAMP;
		case OPtextureWrap::CLAMP_TO_BORDER: return GL_CLAMP_TO_BORDER;
		case OPtextureWrap::CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
		case OPtextureWrap::REPEAT: return GL_REPEAT;
		case OPtextureWrap::MIRRORED_REPEAT: return GL_MIRRORED_REPEAT;
	}
	return 0;
}

ui32 OPtextureFormatToGL(OPtextureFormat textureFormat) {
	switch (textureFormat)
	{
	case OPtextureFormat::RGBA: return GL_RGBA;
	case OPtextureFormat::RGB: return GL_RGB;
	case OPtextureFormat::BGRA: return GL_BGRA;
	case OPtextureFormat::BGR: return GL_BGR;
	case OPtextureFormat::LUMINANCE: return GL_LUMINANCE;
	case OPtextureFormat::LUMINANCE_ALPHA: return GL_LUMINANCE_ALPHA;
	case OPtextureFormat::RGB16F: return GL_RGB16F;
	case OPtextureFormat::RGB32F: return GL_RGB32F;
	case OPtextureFormat::DEPTH: return GL_DEPTH_COMPONENT;
	case OPtextureFormat::DEPTH16F: return GL_DEPTH_COMPONENT16;
	case OPtextureFormat::DEPTH32F: return GL_DEPTH_COMPONENT32F;
	}
	return 0;
}

ui32 OPtextureFilterToGL(OPtextureFilter textureFilter) {
	switch (textureFilter) {
	case OPtextureFilter::LINEAR: return GL_LINEAR;
	case OPtextureFilter::NEAREST: return GL_NEAREST;
	}
	return 0;
}

ui32 OPtextureTypeToGL(OPtextureType textureType) {
	switch (textureType) {
	case OPtextureType::BYTE: return GL_UNSIGNED_BYTE;
	case OPtextureType::FLOAT: return GL_FLOAT;
	}
	return 0;
}

OPtexture* OPtextureGLInit(OPtexture* texture, OPtextureDesc textureDesc, const void* pixelData) {
	OPtextureGL* internalPtr = (OPtextureGL*)OPalloc(sizeof(OPtextureGL));;
	texture->internalPtr = internalPtr;
	texture->textureDesc = textureDesc;

	OPGLFN(glGenTextures(1, &internalPtr->Handle));
	OPGLFN(glActiveTexture(GL_TEXTURE0 + 0));
	OPGLFN(glBindTexture(GL_TEXTURE_2D, internalPtr->Handle));

	ui32 textureInternalFormat = OPtextureFormatToGL(textureDesc.internalFormat);
	ui32 textureFormat = OPtextureFormatToGL(textureDesc.format);
	ui32 textureFilter = OPtextureFilterToGL(textureDesc.filter);
	ui32 textureWrap = OPtextureWrapToGL(textureDesc.wrap);
	ui32 textureType = OPtextureTypeToGL(textureDesc.textureType);

	OPGLFN(glTexImage2D(GL_TEXTURE_2D, 0, textureInternalFormat, textureDesc.width, textureDesc.height, 0, textureFormat, textureType, pixelData));
	OPGLFN(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilter));
	OPGLFN(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilter));
	OPGLFN(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrap));
	OPGLFN(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrap));

	float aniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);

	OPGLFN(glGenerateMipmap(GL_TEXTURE_2D));

	return texture;
}

OPtexture* OPvertexBufferGLCreate(OPtextureDesc textureDesc, const void* pixelData) {
	OPtexture* texture = (OPtexture*)OPalloc(sizeof(OPtexture));
	return OPtextureGLInit(texture, textureDesc, pixelData);
}

void OPtextureGLSetData(OPtexture* ptr, const void* pixelData) {
	OPtextureGL* texture = (OPtextureGL*)ptr->internalPtr;
	OPGLFN(glBindTexture(GL_TEXTURE_2D, texture->Handle));
	OPGLFN(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, ptr->textureDesc.width, ptr->textureDesc.height, OPtextureFormatToGL(ptr->textureDesc.format), GL_UNSIGNED_BYTE, pixelData));
}

void OPtextureGLDestroy(OPtexture* ptr) {
	OPtextureGL* textureGL = (OPtextureGL*)ptr->internalPtr;
	OPGLFN(glDeleteTextures(1, &textureGL->Handle));
	OPfree(textureGL);
	ptr->internalPtr = NULL;
}

void OPtextureGLBind(OPtexture* ptr, ui32 slot) {
	OPtextureGL* texture = (OPtextureGL*)ptr->internalPtr;
	OPGLFN(glActiveTexture(GL_TEXTURE0 + slot));
	OPGLFN(glBindTexture(GL_TEXTURE_2D, texture->Handle));
}

void OPtextureGLUnbind(OPtexture* ptr, ui32 slot) {
	OPGLFN(glActiveTexture(GL_TEXTURE0 + slot));
	OPGLFN(glBindTexture(GL_TEXTURE_2D, 0));
}

void OPtextureAPIGLInit(OPtextureAPI* texture) {
	texture->_Init = OPtextureGLInit;
	texture->_Create = OPvertexBufferGLCreate;
	texture->SetData = OPtextureGLSetData;
	texture->Bind = OPtextureGLBind;
	texture->Unbind = OPtextureGLUnbind;
	texture->Destroy = OPtextureGLDestroy;
}

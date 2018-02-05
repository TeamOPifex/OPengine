
#include "./Human/include/Platform/opengl/OPtextureCubeAPIGL.h"

#ifdef OPIFEX_OPENGL

#include "./Human/include/Platform/opengl/OPcommonGL.h"
#include "./Human/include/Rendering/OPtextureCube.h"


OPtextureCube* OPtextureCubeGLInit(OPtextureCube* textureCube, OPimage* textures) {
    OPtextureCubeGL* textureCubeGL = OPNEW(OPtextureCubeGL());
    textureCube->internalPtr = textureCubeGL;

    OPGLFN(glGenTextures(1, &textureCubeGL->Handle));
    OPGLFN(glBindTexture(GL_TEXTURE_CUBE_MAP, textureCubeGL->Handle));

    ui32 internalFormat = GL_RGBA;
    ui32 format = GL_RGBA;

	for (ui32 i = 0; i < 6; i++) {
		if (textures[i].Components == 3) {
			internalFormat = GL_RGB;
			format = GL_RGB;
		}
		else {
			internalFormat = GL_RGBA;
			format = GL_RGBA;
		}
		OPGLFN(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, textures[i].Width, textures[i].Height, 0, format, GL_UNSIGNED_BYTE, textures[i].Data));
	}

    // OPGLFN(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
	OPGLFN(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	OPGLFN(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	OPGLFN(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	OPGLFN(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	OPGLFN(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

    return textureCube;
}

OPtextureCube* OPtextureCubeGLInitDesc(OPtextureCube* textureCube, OPtextureDesc desc) {
	OPtextureCubeGL* textureCubeGL = OPNEW(OPtextureCubeGL());
	textureCube->internalPtr = textureCubeGL;

	OPGLFN(glGenTextures(1, &textureCubeGL->Handle));
	OPGLFN(glBindTexture(GL_TEXTURE_CUBE_MAP, textureCubeGL->Handle));


	ui32 textureInternalFormat = OPtextureFormatToGL(desc.internalFormat);
	ui32 textureFormat = OPtextureFormatToGL(desc.format);
	ui32 textureMinFilter = OPtextureFilterToGL(desc.minfilter);
	ui32 textureMagFilter = OPtextureFilterToGL(desc.magfilter);
	ui32 textureWrap = OPtextureWrapToGL(desc.wrap);
	ui32 textureType = OPtextureTypeToGL(desc.textureType);

	for (ui32 i = 0; i < 6; i++) {
		OPGLFN(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, textureInternalFormat, desc.width, desc.height, 0, textureFormat, textureType, NULL));
	}

	OPGLFN(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, textureMinFilter));
	OPGLFN(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, textureMagFilter));
	OPGLFN(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, textureWrap));
	OPGLFN(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, textureWrap));
	OPGLFN(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, textureWrap));

	if (desc.mipmap) {
		OPGLFN(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
	}

	return textureCube;
}

OPtextureCube* OPtextureCubeGLCreate(OPimage* textures) {
    OPtextureCube* result = OPNEW(OPtextureCube());
    OPtextureCubeGLInit(result, textures);
    return result;
}

void OPtextureCubeGLGenMipMaps(OPtextureCube* textureCube) {
	OPtextureCubeGL* textureCubeGL = (OPtextureCubeGL*)textureCube->internalPtr;
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureCubeGL->Handle);
	OPGLFN(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
}

void OPtextureCubeGLBind(OPtextureCube* textureCube, ui32 slot) {
    OPtextureCubeGL* textureCubeGL = (OPtextureCubeGL*)textureCube->internalPtr;
    OPGLFN(glActiveTexture(GL_TEXTURE0 + slot));
    OPGLFN(glBindTexture(GL_TEXTURE_CUBE_MAP, textureCubeGL->Handle));
}

void OPtextureCubeGLBindSide(OPtextureCube* textureCube, ui32 side, ui32 mip, ui32 slot) {
	OPtextureCubeGL* textureCubeGL = (OPtextureCubeGL*)textureCube->internalPtr;

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + slot,
		GL_TEXTURE_CUBE_MAP_POSITIVE_X + side, textureCubeGL->Handle, mip);
}

void OPtextureCubeGLUnbind(OPtextureCube* textureCube, ui32 slot) {
    OPGLFN(glActiveTexture(GL_TEXTURE0 + slot));
    OPGLFN(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

void OPtextureCubeGLDestroy(OPtextureCube* textureCube) {
    OPtextureCubeGL* textureCubeGL = (OPtextureCubeGL*)textureCube->internalPtr;
    OPGLFN(glDeleteTextures(1, &textureCubeGL->Handle));
	OPfree(textureCubeGL);
	textureCube->internalPtr = NULL;
}

void OPtextureCubeAPIGLInit(OPtextureCubeAPI* textureCube) {
	textureCube->Bind = OPtextureCubeGLBind;
	textureCube->GenMipMaps = OPtextureCubeGLGenMipMaps;
	textureCube->BindSide = OPtextureCubeGLBindSide;
	textureCube->Destroy = OPtextureCubeGLDestroy;
	textureCube->Unbind = OPtextureCubeGLUnbind;
	textureCube->_Create = OPtextureCubeGLCreate;
	textureCube->_Init = OPtextureCubeGLInit;
	textureCube->_InitDesc = OPtextureCubeGLInitDesc;
	
}

#endif

#include "./Human/include/Platform/opengles/OPtextureCubeAPIGLES.h"

#ifdef OPIFEX_OPENGL_ES_2

#include "./Human/include/Platform/opengles/OPcommonGLES.h"
#include "./Human/include/Rendering/OPtextureCube.h"


OPtextureCube* OPtextureCubeGLESInit(OPtextureCube* textureCube, OPimage* textures) {
    OPtextureCubeGLES* textureCubeGL = OPNEW(OPtextureCubeGLES());
    textureCube->internalPtr = textureCubeGL;

    OPGLFN(glGenTextures(1, &textureCubeGL->Handle));
    OPGLFN(glBindTexture(GL_TEXTURE_CUBE_MAP, textureCubeGL->Handle));
    OPGLFN(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    OPGLFN(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    OPGLFN(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    OPGLFN(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    ui32 internalFormat = GL_RGBA;
    ui32 format = GL_RGBA;

    OPGLFN(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internalFormat, textures[0].Width, textures[0].Height, 0, format, GL_UNSIGNED_BYTE, textures[0].Data));
    OPGLFN(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internalFormat, textures[1].Width, textures[1].Height, 0, format, GL_UNSIGNED_BYTE, textures[1].Data));

    OPGLFN(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internalFormat, textures[2].Width, textures[2].Height, 0, format, GL_UNSIGNED_BYTE, textures[2].Data));
    OPGLFN(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internalFormat, textures[3].Width, textures[3].Height, 0, format, GL_UNSIGNED_BYTE, textures[3].Data));

    OPGLFN(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internalFormat, textures[4].Width, textures[4].Height, 0, format, GL_UNSIGNED_BYTE, textures[4].Data));
    OPGLFN(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internalFormat, textures[5].Width, textures[5].Height, 0, format, GL_UNSIGNED_BYTE, textures[5].Data));
    
    OPGLFN(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));

    return textureCube;
}

OPtextureCube* OPtextureCubeGLESCreate(OPimage* textures) {
    OPtextureCube* result = OPNEW(OPtextureCube());
    OPtextureCubeGLESInit(result, textures);
    return result;
}

void OPtextureCubeGLESBind(OPtextureCube* textureCube, ui32 slot) {
    OPtextureCubeGLES* textureCubeGL = (OPtextureCubeGLES*)textureCube->internalPtr;
    OPGLFN(glActiveTexture(GL_TEXTURE0 + slot));
    OPGLFN(glBindTexture(GL_TEXTURE_CUBE_MAP, textureCubeGL->Handle));
}

void OPtextureCubeGLESUnbind(OPtextureCube* textureCube, ui32 slot) {
    OPGLFN(glActiveTexture(GL_TEXTURE0 + slot));
    OPGLFN(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

void OPtextureCubeGLESDestroy(OPtextureCube* textureCube) {
    OPtextureCubeGLES* textureCubeGL = (OPtextureCubeGLES*)textureCube->internalPtr;
    OPGLFN(glDeleteTextures(1, &textureCubeGL->Handle));
	OPfree(textureCubeGL);
	textureCube->internalPtr = NULL;
}

void OPtextureCubeAPIGLESInit(OPtextureCubeAPI* textureCube) {
	textureCube->Bind = OPtextureCubeGLESBind;
	textureCube->Destroy = OPtextureCubeGLESDestroy;
	textureCube->Unbind = OPtextureCubeGLESUnbind;
	textureCube->_Create = OPtextureCubeGLESCreate;
	textureCube->_Init = OPtextureCubeGLESInit;
}

#endif
#include "./Human/include/Platform/opengl/OPtextureCubeAPIGL.h"
#include "./Human/include/Platform/opengl/OPcommonGL.h"
#include "./Human/include/Rendering/OPtextureCube.h"


OPtextureCube* OPtextureCubeGLInit(OPtextureCube* textureCube, OPimage* textures) {
    OPtextureCubeGL* textureCubeGL = OPNEW(OPtextureCubeGL());
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

OPtextureCube* OPtextureCubeGLCreate(OPimage* textures) {
    OPtextureCube* result = OPNEW(OPtextureCube());
    OPtextureCubeGLInit(result, textures);
    return result;
}

void OPtextureCubeGLBind(OPtextureCube* textureCube, ui32 slot) {
    OPtextureCubeGL* textureCubeGL = (OPtextureCubeGL*)textureCube->internalPtr;
    OPGLFN(glActiveTexture(GL_TEXTURE0 + slot));
    OPGLFN(glBindTexture(GL_TEXTURE_CUBE_MAP, textureCubeGL->Handle));
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
	textureCube->Destroy = OPtextureCubeGLDestroy;
	textureCube->Unbind = OPtextureCubeGLUnbind;
	textureCube->_Create = OPtextureCubeGLCreate;
	textureCube->_Init = OPtextureCubeGLInit;
}

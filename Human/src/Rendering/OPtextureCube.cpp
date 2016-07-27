#include "./Human/include/Rendering/OPtextureCube.h"
#include "./Human/include/Utilities/Errors.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/Assert.h"

void OPtextureCubeInit(OPtextureCubeOLD *result, OPimage *faces) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    if (OPglError("OPtextureCubeInit::Error 1")) {
        OPlog("FAILED to make cube map texture");
    }

    for(GLuint i = 0; i < 6; i++)
    {
        glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                GL_RGBA, faces[i].Width, faces[i].Height, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, faces[i].Data
        );

        OPlog("Face %d, %d", faces[i].Width, faces[i].Height);

        if (OPglError("OPtextureCubeInit::Error 1.5")) {
            OPlog("FAILED to gen image for face %d", i);
        }
    }

    if (OPglError("OPtextureCubeInit::Error 2")) {
        OPlog("FAILED to make cube map texture");
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#ifndef OPIFEX_IOS
    // TODO: (garrett) Fix cubemap on iOS
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
#endif
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    if (OPglError("OPtextureCubeInit::Error 3")) {
        OPlog("FAILED to make cube map texture");
    }

    //result->Texture.Handle = textureID;

    return;
    //return textureID;
}


OPtextureCubeOLD* OPtextureCubeCreate(OPimage * faces) {
	OPtextureCubeOLD* result = (OPtextureCubeOLD*)OPalloc(sizeof(OPtextureCubeOLD));
    OPtextureCubeInit(result, faces);
    return result;
}

void OPtextureCubeDestroy(OPtextureCubeOLD * result) {

}

void OPtextureCubeFree(OPtextureCubeOLD * result) {
    OPtextureCubeDestroy(result);
    OPfree(result);
}

ui32 OPTEXTURECUBE_ACTIVE = 0;

void OPtextureCubeClearActive() {
    OPTEXTURECUBE_ACTIVE = 0;
}

ui32 OPtextureCubeBind(OPtextureCubeOLD* result) {
    ASSERT(OPTEXTURECUBE_ACTIVE < 16, "Exceeded Active Texture Slots");
	OPRENDERER_ACTIVE->Texture.Bind(&result->Texture, OPTEXTURECUBE_ACTIVE);
    return OPTEXTURECUBE_ACTIVE++;
}
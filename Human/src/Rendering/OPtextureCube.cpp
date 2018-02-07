#include "./Human/include/Rendering/OPtextureCube.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/Assert.h"
//
void OPtextureCubeInit(OPtextureCubeOLD *result, OPimage *faces) {
//    GLuint textureID;
//    glGenTextures(1, &textureID);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
//
//
//    for(GLuint i = 0; i < 6; i++)
//    {
//        glTexImage2D(
//                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
//                GL_RGBA, faces[i].Width, faces[i].Height, 0,
//                GL_RGBA, GL_UNSIGNED_BYTE, faces[i].Data
//        );
//
//        OPlog("Face %d, %d", faces[i].Width, faces[i].Height);
//
//    }
//
//
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//#ifndef OPIFEX_IOS
//    // TODO: (garrett) Fix cubemap on iOS
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//#endif
//    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
//
//    //result->Texture.Handle = textureID;
//
//    return;
//    //return textureID;
}
//
//
//OPtextureCubeOLD* OPtextureCubeCreate(OPimage * faces) {
//	OPtextureCubeOLD* result = (OPtextureCubeOLD*)OPalloc(sizeof(OPtextureCubeOLD));
//    OPtextureCubeInit(result, faces);
//    return result;
//}
//
//void OPtextureCubeDestroy(OPtextureCubeOLD * result) {
//
//}
//
//void OPtextureCubeFree(OPtextureCubeOLD * result) {
//    OPtextureCubeDestroy(result);
//    OPfree(result);
//}
//
//ui32 OPTEXTURECUBE_ACTIVE = 0;
//
void OPtextureCubeClearActive() {
	//    OPTEXTURECUBE_ACTIVE = 0;
}
//
//ui32 OPtextureCubeBind(OPtextureCubeOLD* result) {
//    ASSERT(OPTEXTURECUBE_ACTIVE < 16, "Exceeded Active Texture Slots");
//	OPRENDERER_ACTIVE->Texture.Bind(&result->Texture, OPTEXTURECUBE_ACTIVE);
//    return OPTEXTURECUBE_ACTIVE++;
//}

#include "./Human/include/Rendering/Primitives/OPcube.h"
#include "./Human/include/Rendering/OPmodel.h"
#include "./Human/include/Rendering/OPframeBuffer.h"

void OPtextureCube::Process(OPtextureCube* textureCube, OPeffect* effect, OPtexture* texture, const OPchar* param, ui32 size) {

	OPrenderDepth(false);
	OPrenderCull(false);

	OPframeBuffer captureFB;

	OPtextureDesc textureCubeDesc = OPtextureDesc(size, size);
	textureCubeDesc.internalFormat = OPtextureFormat::RGB16F;
	textureCubeDesc.format = OPtextureFormat::RGB;
	textureCubeDesc.textureType = OPtextureType::FLOAT;
	textureCubeDesc.wrap = OPtextureWrap::CLAMP_TO_EDGE;
	textureCubeDesc.minfilter = OPtextureFilter::LINEAR_MIPMAP;
	textureCubeDesc.magfilter = OPtextureFilter::LINEAR;
	textureCubeDesc.mipmap = true;
	textureCube->Init(textureCubeDesc);


	// TODO: (garrett) create a non allocating OPmodel version of OPcubeCreate
	OPmodel* cubeMesh = OPcubeCreate();

	OPtextureDesc captureFBDesc = OPtextureDesc(size, size);
	captureFB.Init(captureFBDesc, false);


	OPcam views[6];
	views[0].SetPerspective(OPVEC3_ZERO, OPvec3(1, 0, 0), OPvec3(0, -1, 0), 0.1f, 10.0f, 90.0f, 1.0);
	views[1].SetPerspective(OPVEC3_ZERO, OPvec3(-1, 0, 0), OPvec3(0, -1, 0), 0.1f, 10.0f, 90.0f, 1.0);
	views[2].SetPerspective(OPVEC3_ZERO, OPvec3(0, 1, 0), OPvec3(0, 0, 1), 0.1f, 10.0f, 90.0f, 1.0);
	views[3].SetPerspective(OPVEC3_ZERO, OPvec3(0, -1, 0), OPvec3(0, 0, -1), 0.1f, 10.0f, 90.0f, 1.0);
	views[4].SetPerspective(OPVEC3_ZERO, OPvec3(0, 0, 1), OPvec3(0, -1, 0), 0.1f, 10.0f, 90.0f, 1.0);
	views[5].SetPerspective(OPVEC3_ZERO, OPvec3(0, 0, -1), OPvec3(0, -1, 0), 0.1f, 10.0f, 90.0f, 1.0);


	captureFB.Bind();

	effect->Set("uProj", &views[0].proj);
	effect->Set(param, texture, 0);

	for (ui32 i = 0; i < 6; i++) {
		effect->Set("uView", &views[i].view);

		textureCube->Bind(i, 0);

		OPrenderClear(0, 0, 0);


		cubeMesh->Bind();
		OPrenderDrawBufferIndexed(0);

	}

	captureFB.Unbind();

	textureCube->GenMipMaps();


	captureFB.Destroy();

	cubeMesh->Destroy();
	OPfree(cubeMesh);

}


void OPtextureCube::Process(OPtextureCube* textureCube, OPeffect* effect, OPtextureCube* texture, const OPchar* param, ui32 size) {
	OPrenderDepth(false);
	OPrenderCull(false);

	//OPtextureCube textureCube;
	OPframeBuffer captureFB;

	OPtextureDesc textureCubeDesc = OPtextureDesc(size, size);
	textureCubeDesc.internalFormat = OPtextureFormat::RGB16F;
	textureCubeDesc.format = OPtextureFormat::RGB;
	textureCubeDesc.textureType = OPtextureType::FLOAT;
	textureCubeDesc.wrap = OPtextureWrap::CLAMP_TO_EDGE;
	textureCubeDesc.minfilter = OPtextureFilter::LINEAR;
	textureCubeDesc.magfilter = OPtextureFilter::LINEAR;
	// textureCubeDesc.mipmap = true;
	textureCube->Init(textureCubeDesc);


	// TODO: (garrett) create a non allocating OPmodel version of OPcubeCreate
	OPmodel* cubeMesh = OPcubeCreate();

	OPtextureDesc captureFBDesc = OPtextureDesc(size, size);
	captureFB.Init(captureFBDesc, false);

	OPcam views[6];
	views[0].SetPerspective(OPVEC3_ZERO, OPvec3(1, 0, 0), OPvec3(0, -1, 0), 0.1f, 10.0f, 90.0f, 1.0);
	views[1].SetPerspective(OPVEC3_ZERO, OPvec3(-1, 0, 0), OPvec3(0, -1, 0), 0.1f, 10.0f, 90.0f, 1.0);
	views[2].SetPerspective(OPVEC3_ZERO, OPvec3(0, 1, 0), OPvec3(0, 0, 1), 0.1f, 10.0f, 90.0f, 1.0);
	views[3].SetPerspective(OPVEC3_ZERO, OPvec3(0, -1, 0), OPvec3(0, 0, -1), 0.1f, 10.0f, 90.0f, 1.0);
	views[4].SetPerspective(OPVEC3_ZERO, OPvec3(0, 0, 1), OPvec3(0, -1, 0), 0.1f, 10.0f, 90.0f, 1.0);
	views[5].SetPerspective(OPVEC3_ZERO, OPvec3(0, 0, -1), OPvec3(0, -1, 0), 0.1f, 10.0f, 90.0f, 1.0);


	captureFB.Bind();

	effect->Set("uProj", &views[0].proj);
	effect->Set(param, texture, 0);

	for (ui32 i = 0; i < 6; i++) {
		effect->Set("uView", &views[i].view);

		textureCube->Bind(i, 0);

		OPrenderClear(0, 0, 0);

		cubeMesh->Bind();
		OPrenderDrawBufferIndexed(0);

	}

	captureFB.Unbind();


	captureFB.Destroy();

	cubeMesh->Destroy();
	OPfree(cubeMesh);

}




void OPtextureCube::ProcessRoughness(OPtextureCube* textureCube, OPeffect* effect, OPtextureCube* texture, const OPchar* param, ui32 size) {
	OPrenderDepth(false);
	OPrenderCull(false);

	//OPtextureCube textureCube;
	OPframeBuffer captureFB;

	OPtextureDesc textureCubeDesc = OPtextureDesc(size, size);
	textureCubeDesc.internalFormat = OPtextureFormat::RGB16F;
	textureCubeDesc.format = OPtextureFormat::RGB;
	textureCubeDesc.textureType = OPtextureType::FLOAT;
	textureCubeDesc.wrap = OPtextureWrap::CLAMP_TO_EDGE;
	textureCubeDesc.minfilter = OPtextureFilter::LINEAR_MIPMAP;
	textureCubeDesc.magfilter = OPtextureFilter::LINEAR;
	textureCubeDesc.mipmap = true;
	textureCube->Init(textureCubeDesc);


	// TODO: (garrett) create a non allocating OPmodel version of OPcubeCreate
	OPmodel* cubeMesh = OPcubeCreate();

	OPtextureDesc captureFBDesc = OPtextureDesc(size, size);
	captureFB.Init(captureFBDesc, false);

	OPcam views[6];
	views[0].SetPerspective(OPVEC3_ZERO, OPvec3(1, 0, 0), OPvec3(0, -1, 0), 0.1f, 10.0f, 90.0f, 1.0);
	views[1].SetPerspective(OPVEC3_ZERO, OPvec3(-1, 0, 0), OPvec3(0, -1, 0), 0.1f, 10.0f, 90.0f, 1.0);
	views[2].SetPerspective(OPVEC3_ZERO, OPvec3(0, 1, 0), OPvec3(0, 0, 1), 0.1f, 10.0f, 90.0f, 1.0);
	views[3].SetPerspective(OPVEC3_ZERO, OPvec3(0, -1, 0), OPvec3(0, 0, -1), 0.1f, 10.0f, 90.0f, 1.0);
	views[4].SetPerspective(OPVEC3_ZERO, OPvec3(0, 0, 1), OPvec3(0, -1, 0), 0.1f, 10.0f, 90.0f, 1.0);
	views[5].SetPerspective(OPVEC3_ZERO, OPvec3(0, 0, -1), OPvec3(0, -1, 0), 0.1f, 10.0f, 90.0f, 1.0);


	captureFB.Bind();

	effect->Set("uProj", &views[0].proj);
	effect->Set(param, texture, 0);

	unsigned int maxMipLevels = 5;
	for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
	{
		// reisze framebuffer according to mip-level size.
		ui32 mipWidth = 128 * OPpow(0.5, mip);
		ui32 mipHeight = 128 * OPpow(0.5, mip);

		captureFB.Bind(mip);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		effect->Set("uRoughness", roughness);

		for (unsigned int i = 0; i < 6; ++i)
		{
			effect->Set("uView", &views[i].view);
			textureCube->Bind(i, mip, 0);

			OPrenderClear(0, 0, 0);

			cubeMesh->Bind();
			OPrenderDrawBufferIndexed(0);
		}
	}

	captureFB.Unbind();


	captureFB.Destroy();

	cubeMesh->Destroy();
	OPfree(cubeMesh);

}

void OPtextureCube::FromEquirectangular(OPtextureCube* textureCube, OPtexture* texture, ui32 size) {
	OPeffect EquirectangularToCubeMapEffect;	

	EquirectangularToCubeMapEffect.Init("EquirectangularToCubeMap.vert", "EquirectangularToCubeMap.frag");
	EquirectangularToCubeMapEffect.Bind();
	EquirectangularToCubeMapEffect.Set("uEquirectangularMap", texture, 0);

	OPtextureCube::Process(textureCube, &EquirectangularToCubeMapEffect, texture, "uEquirectangularMap", size);

	EquirectangularToCubeMapEffect.Destroy();
}

void OPtextureCube::Convolute(OPtextureCube* textureCube, OPtextureCube* texture, ui32 size) {
	OPeffect ConvoluteEffect;

	ConvoluteEffect.Init("Convolute.vert", "Convolute.frag");
	ConvoluteEffect.Bind();

	OPtextureCube::Process(textureCube, &ConvoluteEffect, texture, "uEnvironmentMap", size);
	
	ConvoluteEffect.Destroy();
}

void OPtextureCube::RoughnessMap(OPtextureCube* textureCube, OPtextureCube* texture, ui32 size) {
	OPeffect HDRFilterRoughnessEffect;

	HDRFilterRoughnessEffect.Init("HDRFilterRoughness.vert", "HDRFilterRoughness.frag");
	HDRFilterRoughnessEffect.Bind();

	OPtextureCube::ProcessRoughness(textureCube, &HDRFilterRoughnessEffect, texture, "uEnvironmentMap", size);

	HDRFilterRoughnessEffect.Destroy();
}
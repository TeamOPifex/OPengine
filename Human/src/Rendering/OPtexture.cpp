#include "./Human/include/Rendering/OPtexture.h"
#include "./Data/include/OPcman.h"

OPtexture* OPtextureLoad(const OPchar* asset) {
    return (OPtexture*)OPCMAN.LoadGet(asset);
}


#include "./Human/include/Rendering/Primitives/OPquad.h"
#include "./Human/include/Rendering/OPmodel.h"
#include "./Human/include/Rendering/OPframeBuffer.h"

void OPtexture::GenerateBRDF(OPtexture* texture, ui32 size) {
	OPrenderDepth(false);
	OPrenderCull(false);

	OPtextureDesc desc = OPtextureDesc(size, size);
	desc.wrap = OPtextureWrap::CLAMP_TO_EDGE;
	desc.minfilter = OPtextureFilter::LINEAR;
	desc.magfilter = OPtextureFilter::LINEAR;
	desc.internalFormat = OPtextureFormat::RGBA16F;
	desc.format = OPtextureFormat::RGBA;
	desc.textureType = OPtextureType::FLOAT;
	texture->Init(desc);



	OPmodel* quad = OPquadCreate(1.0, 1.0);


	OPframeBuffer captureFB;
	OPtextureDesc captureFBDesc = OPtextureDesc(size, size);
	captureFB.Init(captureFBDesc, false);



	OPeffect BRDFEffect;
	BRDFEffect.Init("BRDF.vert", "BRDF.frag");

	captureFB.Bind();
	captureFB.SetAttachment(0, texture);
	OPrenderClear(0.0);

	BRDFEffect.Bind();

	quad->Bind();
	OPrenderDrawBufferIndexed(0);

	captureFB.Unbind();
	captureFB.Destroy();

	BRDFEffect.Destroy();
	
	quad->Destroy();
	OPfree(quad);
}
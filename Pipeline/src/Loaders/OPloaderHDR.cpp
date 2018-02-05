#include "./Pipeline/include/Loaders/OPloaderHDR.h"
#include "./Human/include/Utilities/OPimagePNG.h"
#include "./Data/include/OPstring.h"
#include "./Core/include/Assert.h"

#include "./Pipeline/include/Loaders/stb_image.h"


i32 OPimageHDRLoad(OPstream* str, OPtexture** image) {
	if (str == NULL) {
		OPlogErr("Image not found.");
		return NULL;
	}

	stbi_set_flip_vertically_on_load(true);
	i32 width, height, nrComponents;
	//f32 *data = stbi_loadf_from_memory(str->Data, str->Length, &width, &height, &nrComponents, 0);
	f32* data = stbi_loadf(str->Source, &width, &height, &nrComponents, 0);
	//ui8* data = stbi_load(str->Source, &width, &height, &nrComponents, 0);
	//ui8* data = stbi_load_from_memory(str->Data, str->Length, &width, &height, &nrComponents, 0);

	OPtexture* tex = OPNEW(OPtexture());

	OPtextureDesc desc;
	desc.width = width;
	desc.height = height;
	//desc.format = nrComponents == 3 ? OPtextureFormat::RGB : OPtextureFormat::RGBA;
	desc.format = OPtextureFormat::RGB;
	desc.internalFormat = OPtextureFormat::RGB16F;
	desc.textureType = OPtextureType::FLOAT;
	//desc.internalFormat = nrComponents == 3 ? OPtextureFormat::RGB : OPtextureFormat::RGBA;
	//desc.internalFormat = OPtextureFormat::RGBA;
	//desc.textureType = OPtextureType::UBYTE;

	OPRENDERER_ACTIVE->Texture.Init(tex, desc, data);
	
	stbi_image_free(data);

	*image = tex;

	return 1;
}

i32 OPimageHDRReload(OPstream* str, OPtexture** image) {
	OPlog("Reload Image HDR");
	OPtexture* resultTex;
	OPtexture* tex = (OPtexture*)(*image);


	//i32 result = OPimagePNGLoadStream(str, 0, &resultTex);
	//if (result) {
	//	OPmemcpy(*image, resultTex, sizeof(OPtexture));
	//	OPfree(resultTex);
	//}


	return 0;
}

i32 OPimageHDRUnload(OPtexture* image) {
	OPtexture* tex = (OPtexture*)image;
	tex->Destroy();
	OPfree(tex);
	return 1;
}

OPassetLoader OPASSETLOADER_HDR = {
	".hdr",
	"Textures/",
	sizeof(OPtexture),
	(OPint(*)(OPstream*, void**))OPimageHDRLoad,
	(OPint(*)(void*))OPimageHDRUnload,
	(OPint(*)(OPstream*, void**))OPimageHDRReload
};
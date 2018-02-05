#include "./Pipeline/include/Loaders/OPloaderPNG.h"
#include "./Human/include/Utilities/OPimagePNG.h"
#include "./Core/include/Assert.h"


#include "./Pipeline/include/Loaders/stb_image.h"

i32 OPimagePNGLoad(OPstream* str, OPtexture** image) {
	if (str == NULL) {
		OPlogErr("Image not found.");
		return NULL;
	}


	int width, height, nrComponents;
	unsigned char *data = stbi_load_from_memory(str->Data, str->Length, &width, &height, &nrComponents, 0);
	if (data == NULL) {
		OPlogErr("Image failed to load.");
		return NULL;
	}


	OPtexture* tex = OPNEW(OPtexture());

	OPtextureDesc desc;
	desc.width = width;
	desc.height = height;

	if (nrComponents == 1) {
		desc.format = desc.internalFormat = OPtextureFormat::RED;
	}
	else if (nrComponents == 3) {
		desc.format = desc.internalFormat = OPtextureFormat::RGB;
	}
	else if (nrComponents == 4) {
		desc.format = desc.internalFormat =OPtextureFormat::RGBA;
	}
	desc.textureType = OPtextureType::BYTE;

	OPRENDERER_ACTIVE->Texture.Init(tex, desc, data);

	stbi_image_free(data);

	*image = tex;



	return OPimagePNGLoadStream(str, 0, image);
}

i32 OPimagePNGReload(OPstream* str, OPtexture** image) {
	OPlog("Reload Image PNG");
	OPtexture* resultTex;
	OPtexture* tex = (OPtexture*)(*image);
	i32 result = OPimagePNGLoadStream(str, 0, &resultTex);
	if (result) {
		OPmemcpy(*image, resultTex, sizeof(OPtexture));
		OPfree(resultTex);
	}
	return result;
}

i32 OPimagePNGUnload(OPtexture* image) {
	OPtexture* tex = (OPtexture*)image;
	tex->Destroy();
	OPfree(tex);
	return 1;
}

OPassetLoader OPASSETLOADER_PNG = {
	".png",
	"Textures/",
	sizeof(OPtexture),
	(OPint(*)(OPstream*, void**))OPimagePNGLoad,
	(OPint(*)(void*))OPimagePNGUnload,
	(OPint(*)(OPstream*, void**))OPimagePNGReload
};
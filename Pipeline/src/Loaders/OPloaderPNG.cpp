#include "./Pipeline/include/Loaders/OPloaderPNG.h"
#include "./Human/include/Utilities/OPimagePNG.h"
#include "./Core/include/Assert.h"

i32 OPimagePNGLoad(OPstream* str, OPtexture** image) {
	ASSERT(str != NULL, "Image not found.");
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
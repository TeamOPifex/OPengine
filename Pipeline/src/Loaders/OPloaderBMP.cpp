#include "./Pipeline/include/Loaders/OPloaderBMP.h"
#include "./Core/include/Assert.h"

i32 OPimageBMPLoad(OPstream* str, OPtexture** image) {
	ASSERT(str != NULL, "Image not found.");

	ui8* data;
	ui32 width, height;
	ui32 components;

	ui32 length = str->Length;


	ui8* fileInfo = str->Read(54);

	if (fileInfo[0] != 'B' && fileInfo[1] != 'M')
	{
		OPlogErr("Error: Invalid File Format. Bitmap Required.");
		return -1;
	}

	if (fileInfo[28] != 24 && fileInfo[28] != 32)
	{
		OPlogErr("Error: Invalid File Format. 24 or 32 bit Image Required.");
		return -1;
	}

	components = fileInfo[28];
	width = fileInfo[18] + (fileInfo[19] << 8);
	height = fileInfo[22] + (fileInfo[23] << 8);
	ui32 PixelsOffset = fileInfo[10] + (fileInfo[11] << 8);
	ui32 size = ((width * components + 31) / 32) * 4 * height;

	str->Seek(PixelsOffset);

	data = str->Read(size);



	OPtexture* tex = OPNEW(OPtexture());

	ui16 w = width, h = height;
	OPtextureDesc desc;
	desc.width = w;
	desc.height = h;
	if (components == 24) {
		desc.format = OPtextureFormat::BGR;
		desc.internalFormat = OPtextureFormat::RGB;
	}
	else {
		desc.format = OPtextureFormat::BGRA;
		desc.internalFormat = OPtextureFormat::RGBA;
	}
	desc.wrap = OPtextureWrap::REPEAT;
	desc.filter = OPtextureFilter::LINEAR;

	OPRENDERER_ACTIVE->Texture.Init(tex, desc);
	tex->SetData(data);


	*image = tex;


	return 1;
}


i32 OPimageBMPReload(OPstream* str, OPtexture** image) {
	OPlog("Reload Image BMP");
	//OPstream* str = OPreadFile(filename);
	OPtexture* resultTex;
	OPtexture* tex = (OPtexture*)(*image);
	i32 result = OPimageBMPLoad(str, &resultTex);
	if (result) {
		OPmemcpy(*image, resultTex, sizeof(OPtexture));
		OPfree(resultTex);
	}
	return result;
}

i32 OPimageBMPUnload(void* image) {
	OPtexture* tex = (OPtexture*)image;
	tex->Destroy();
	OPfree(tex);
	return 1;
}

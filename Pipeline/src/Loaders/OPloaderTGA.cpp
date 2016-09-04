#include "./Pipeline/include/Loaders/OPloaderTGA.h"
#include "./Core/include/Assert.h"

i32 OPimageTGALoad(OPstream* str, OPtexture** image) {
	ASSERT(str != NULL, "Image not found.");

	ui8* data;
	ui32 width, height;
	ui32 components;
	ui32 size;
	bool compressed = false;
	bool freeUpData = false;


	ui8* header;
	ui8 decompressed[12] = { 0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
	ui8 is_compressed[12] = { 0x0, 0x0, 0xA, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

	header = str->Read(sizeof(ui8) * 18);

	if (!OPmemcmp(decompressed, header, sizeof(decompressed)))
	{
		components = header[16];
		width = header[13] * 256 + header[12];
		height = header[15] * 256 + header[14];
		size = ((width * components + 31) / 32) * 4 * height;

		if ((components != 24) && (components != 32))
		{
			OPlogErr("Invalid File Format. Required: 24 or 32 Bit Image.");
			return -1;
		}

		compressed = false;
		data = str->Read(size);
	}
	else if (!OPmemcmp(is_compressed, header, sizeof(is_compressed)))
	{
		components = header[16];
		width = header[13] * 256 + header[12];
		height = header[15] * 256 + header[14];
		size = ((width * components + 31) / 32) * 4 * height;

		if ((components != 24) && (components != 32))
		{
			OPlogErr("Invalid File Format. Required: 24 or 32 Bit Image.");
			return -2;
		}

		int CurrentByte = 0;
		ui32 CurrentPixel = 0;
		compressed = true;
		ui8 ChunkHeader = { 0 };
		int BytesPerPixel = (components / 8);

		data = OPALLOC(ui8, sizeof(ui32) * BytesPerPixel * width * height);
		freeUpData = true;
		//ImageData.resize(width * height * sizeof(PixelInfo));

		do
		{
			ChunkHeader = str->UI8();

			if (ChunkHeader < 128)
			{
				++ChunkHeader;
				for (int I = 0; I < ChunkHeader; ++I, ++CurrentPixel)
				{
					data[CurrentByte++] = str->UI8();
					data[CurrentByte++] = str->UI8();
					data[CurrentByte++] = str->UI8();
					if (components > 24) data[CurrentByte++] = str->UI8();
				}
			}
			else
			{
				ChunkHeader -= 127;

				for (int I = 0; I < ChunkHeader; ++I, ++CurrentPixel)
				{
					data[CurrentByte++] = str->UI8();
					data[CurrentByte++] = str->UI8();
					data[CurrentByte++] = str->UI8();
					if (components > 24) data[CurrentByte++] = str->UI8();
				}
			}
		} while (CurrentPixel < (width * height));
	}
	else
	{
		OPlogErr("Invalid File Format. Required: 24 or 32 Bit Image.");
		return -3;
	}



	OPtexture* tex = OPNEW(OPtexture());

	ui16 w = width, h = height;
	OPtextureDesc desc;
	desc.width = w;
	desc.height = h;
	if (components == 24) {
		desc.format = OPtextureFormat::RGB;
	} else {
		desc.format = OPtextureFormat::RGBA;
	}
	desc.wrap = OPtextureWrap::REPEAT;
	desc.filter = OPtextureFilter::LINEAR;

	OPRENDERER_ACTIVE->Texture.Init(tex, desc);
	tex->SetData(data);

	if (freeUpData) {
		OPfree(data);
	}

	*image = tex;


	return 1;
}


i32 OPimageTGAReload(OPstream* str, OPtexture** image) {
	OPlog("Reload Image TGA");
	//OPstream* str = OPreadFile(filename);
	OPtexture* resultTex;
	OPtexture* tex = (OPtexture*)(*image);
	i32 result = OPimageTGALoad(str, &resultTex);
	if (result) {
		OPmemcpy(*image, resultTex, sizeof(OPtexture));
		OPfree(resultTex);
	}
	return result;
}

i32 OPimageTGAUnload(void* image) {
	OPtexture* tex = (OPtexture*)image;
	tex->Destroy();
	OPfree(tex);
	return 1;
}

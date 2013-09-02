#pragma once
#include "Image.h"
#include "Texture2D.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
	OPint OPimagePNGLoad(const OPchar* filename, Texture2D** image);
	OPint OPimagePNGUnload(void* image);
#ifdef __cplusplus
}
#endif

class ImagePNG : public Image {
public:
	ImagePNG(FILE* fp, ui32 start, ui32 length);
	static ImagePNG* FromFile(const char* file);
	static Texture2D* TextureFromFile(const char* file);
protected:
	ui8* tmp;
};
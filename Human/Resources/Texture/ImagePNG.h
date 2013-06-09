#pragma once
#include "Image.h"
#include "Texture2D.h"
#include <stdio.h>

class ImagePNG : public Image {
public:
	ImagePNG(FILE* fp);
	static ImagePNG* FromFile(const char* file);
	static Texture2D* TextureFromFile(const char* file);
protected:
	ui8* tmp;
};
#pragma once
#include "Image.h"
#include "Texture2D.h"
#include <stdio.h>

class ImageDDS : public Image {
public:
	ImageDDS(FILE* fp);
	static ImageDDS* FromFile(const char* file);
	static Texture2D* TextureFromFile(const char* file);
protected:
	ui8* tmp;
};
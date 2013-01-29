#pragma once
#include "Image.h"
#include <stdio.h>

class ImageDDS : public Image {
public:
	ImageDDS(FILE* fp);
protected:
	ui8* tmp;
};
#pragma once
#include "Texture.h"
#include <stdio.h>

class TextureDDS : public Texture {
protected:
	ui8* tmp;
public:
	TextureDDS(FILE* fp);
};
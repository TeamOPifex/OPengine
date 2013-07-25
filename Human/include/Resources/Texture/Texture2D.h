#pragma once
#include "./Human/include/Resources/Resource.h"
#include "./Human/include/Resources/Texture/Image.h"

class Texture2D : public Resource {
public:
	Texture2D(ImagePtr texture);
	void Bind(ui32 loc, int slot);
};

#pragma once
#include "./Human/Resources/Resource.h"
#include "./Human/Resources/Texture/Image.h"

class Texture2D : public Resource {
public:
	Texture2D(ImagePtr texture);
	void Bind(ui32 loc, int slot);
};

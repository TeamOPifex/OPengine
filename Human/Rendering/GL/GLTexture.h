#pragma once
#include "./Human/Resources/Texture/Texture.h"

class GLTexture {
private:
	ui32 _textureID;
public:
	GLTexture(Texture* texture);
	void bind(ui32 loc, int slot);
	ui32 textureID() { return _textureID; }
};

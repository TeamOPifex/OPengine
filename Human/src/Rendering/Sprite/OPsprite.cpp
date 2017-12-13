#include "./Human/include/Rendering/Sprite/OPsprite.h"
#include "./Human/include/Rendering/Sprite/OPspriteSheet.h"

OPvec2 OPsprite::FrameSize() {
	f32 sheetWidth = (f32)SpriteSheet->Texture->textureDesc.width;
	f32 sheetHeight = (f32)SpriteSheet->Texture->textureDesc.height;
	f32 frameWidth = (Frames[Frame].Size.x * sheetWidth);
	f32 frameHeight = (Frames[Frame].Size.y * sheetHeight);
	return OPvec2(frameWidth, frameHeight);
}

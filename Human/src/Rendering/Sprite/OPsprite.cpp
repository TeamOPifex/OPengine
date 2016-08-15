#include "./Human/include/Rendering/Sprite/OPsprite.h"

OPvec2 OPsprite::FrameSize() {
	f32 sheetWidth = (f32)Sheet->textureDesc.width;
	f32 sheetHeight = (f32)Sheet->textureDesc.height;
	f32 frameWidth = (Frames[Frame].Size.x * sheetWidth);
	f32 frameHeight = (Frames[Frame].Size.y * sheetHeight);
	return OPvec2(frameWidth, frameHeight);
}

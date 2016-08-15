#pragma once

struct OPsprite;
typedef struct OPsprite OPsprite;

#include "./Human/include/Rendering/Sprite/OPspriteFrame.h"
#include "./Human/include/Rendering/OPtexture.h"

// Sprite animation frames
struct OPsprite {
	OPint          FrameCount;
	OPspriteFrame* Frames;  // pointer to animation's frames
	OPint          Frame;   // index of the current frame
	OPfloat        Elapsed; // time since last frame change
	OPtexture*     Sheet;   // texture that this sprite will use
	
	OPvec2 FrameSize();
};
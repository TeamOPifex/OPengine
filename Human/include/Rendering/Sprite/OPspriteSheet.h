#pragma once

struct OPspriteSheet;
struct OPspriteFrame;
struct OPsprite;

typedef struct OPspriteSheet OPspriteSheet;
typedef struct OPspriteFrame OPspriteFrame;
typedef struct OPsprite OPsprite;

#include "./Human/include/Rendering/OPtexture.h"
#include "./Math/include/OPvec2.h"
#include "./Data/include/OPstream.h"

// Used to maintain and clean up sprites that
// have been stored in the manager
struct OPspriteSheet {
	OPint      Width;
	OPint      Height;
	OPint      Sprites;
	OPchar**   Names;
	OPtexture* Sheet;
};

// Actual sprite offset data
struct OPspriteFrame {
	OPvec2  Offset;
	OPvec2  Size;
	OPfloat Delay; // TODO - implement in exporter
};

// Sprite animation frames
struct OPsprite {
	OPint          FrameCount;
	OPspriteFrame* Frames;  // pointer to animation's frames
	OPint          Frame;   // index of the current frame
	OPfloat        Elapsed; // time since last frame change
	OPtexture*     Sheet;   // texture that this sprite will use
};

OPint OPspriteSheetLoad(OPstream* str, OPspriteSheet** ss);
OPint OPspriteSheetUnload(void* ss);
OPvec2 OPspriteCurrentFrameSize(OPsprite* sprite);
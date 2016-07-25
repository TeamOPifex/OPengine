#ifndef OPENGINE_HUMAN_SPRITESHEET
#define OPENGINE_HUMAN_SPRITESHEET

#include "./Core/include/OPtypes.h"
#include "./Data/include/OPfile.h"
#include "./Math/include/OPvec2.h"
#include "./Human/include/Rendering/OPtexture.h"

//-----------------------------------------------------------------------------
//     _____ _                   _
//    / ____| |                 | |
//   | (___ | |_ _ __ _   _  ___| |_ ___
//    \___ \| __| '__| | | |/ __| __/ __|
//    ____) | |_| |  | |_| | (__| |_\__ \
//   |_____/ \__|_|   \__,_|\___|\__|___/
//
//
// Used to maintain and clean up sprites that
// have been stored in the manager
struct OPspriteSheet {
	OPint      Width;
	OPint      Height;
	OPint      Sprites;
	OPchar**   Names;
	OPtextureOLD* Sheet;
};
typedef struct OPspriteSheet OPspriteSheet;

// Actual sprite offset data
struct OPspriteFrame {
	OPvec2  Offset;
	OPvec2  Size;
	OPfloat Delay; // TODO - implement in exporter
};
typedef struct OPspriteFrame OPspriteFrame;

// Sprite animation frames
struct OPsprite {
	OPint          FrameCount;
	OPspriteFrame* Frames;  // pointer to animation's frames
	OPint          Frame;   // index of the current frame
	OPfloat        Elapsed; // time since last frame change
	OPtextureOLD*     Sheet;   // texture that this sprite will use
};
typedef struct OPsprite OPsprite;

//-----------------------------------------------------------------------------
//    ______                _   _
//   |  ____|              | | (_)
//   | |__ _   _ _ __   ___| |_ _  ___  _ __  ___
//   |  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//   | |  | |_| | | | | (__| |_| | (_) | | | \__ \
//   |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//
//
OPint OPspriteSheetLoad(OPstream* str, OPspriteSheet** ss);
OPint OPspriteSheetUnload(void* ss);
OPvec2 OPspriteCurrentFrameSize(OPsprite* sprite);

#endif

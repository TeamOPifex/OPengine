#ifndef OPENGINE_HUMAN_SPRITESHEET
#define OPENGINE_HUMAN_SPRITESHEET

#include "./Core/include/Types.h"
#include "./Math/include/Vector2.h"
#include "./Human/include/Rendering/Texture.h"

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
typedef struct{
	OPint      Sprites;
	OPchar**   Names;
	OPtexture* Sheet;
}OPspriteSheet;

// Actual sprite offset data
typedef struct{
	OPvec2  Offset;
	OPvec2  Size;
	OPfloat Delay; // TODO - implement in exporter
}OPspriteFrame;

// Sprite animation frames
typedef struct{
	OPint          FrameCount;
	OPspriteFrame* Frames;  // pointer to animation's frames
	OPint          Frame;   // index of the current frame
	OPfloat        Elapsed; // time since last frame change
}OPsprite;

//-----------------------------------------------------------------------------
//    ______                _   _                 
//   |  ____|              | | (_)                
//   | |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//   |  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//   | |  | |_| | | | | (__| |_| | (_) | | | \__ \
//   |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//                                                
//                                                
OPint OPspriteSheetLoad(const OPchar* filename, OPspriteSheet** ss);
OPint OPspriteSheetUnload(void* ss);

#endif

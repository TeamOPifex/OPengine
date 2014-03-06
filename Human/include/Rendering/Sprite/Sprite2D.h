#ifndef OPENGINE_HUMAN_SPRITE2D
#define OPENGINE_HUMAN_SPRITE2D

#include "./Human/include/Rendering/Sprite/SpriteSheet.h"
#include "./Human/include/Rendering/Effect.h"

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
	OPvec2      Position;
	OPfloat     Rotation;
	OPvec2      Scale;
	OPsprite	Sprite;
} OPsprite2D;

//-----------------------------------------------------------------------------
//    ______                _   _                 
//   |  ____|              | | (_)                
//   | |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//   |  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//   | |  | |_| | | | | (__| |_| | (_) | | | \__ \
//   |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//                                                
//                               
void OPsprite2DInit(OPeffect* effect); 
void OPsprite2DShutdown();
OPsprite2D* OPsprite2DCreate(OPsprite* sprite);
void OPsprite2DDestroy(OPsprite2D* sprite);
void OPsprite2DBind(OPsprite2D* sprite);
void OPsprite2DRender();

void OPsprite2DMove(OPvec2 xy);
void OPsprite2DMoveX(OPfloat x);
void OPsprite2DMoveY(OPfloat y);
void OPsprite2DRotate(OPfloat r);
void OPsprite2DScale(OPvec2 xy);
void OPsprite2DScaleX(OPfloat x);
void OPsprite2DScaleY(OPfloat y);

#endif

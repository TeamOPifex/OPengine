#pragma once

#include "./Human/include/Rendering/OPfont.h"
#include "./Human/include/Rendering/OPeffect.h"
#include "./Data/include/OPhashMap.h"
#include "./Human/include/Rendering/OPmeshPacked.h"
#include "./Math/include/Vector2.h"
#include "./Math/include/Vector3.h"
#include "./Math/include/Vector4.h"


//-----------------------------------------------------------------------------
//   _____ _                   _       
//  / ____| |                 | |      
// | (___ | |_ _ __ _   _  ___| |_ ___ 
//  \___ \| __| '__| | | |/ __| __/ __|
//  ____) | |_| |  | |_| | (__| |_\__ \
// |_____/ \__|_|   \__,_|\___|\__|___/
typedef struct {
	i8* Text;
	OPint Width;
} OPfontTextNode;


typedef enum {
	OPFONT_ALIGN_LEFT = 0,
	OPFONT_ALIGN_CENTER = 1,
	OPFONT_ALIGN_RIGHT = 2
} OPfontAlign;

typedef struct {
	OPfont* _font;
	OPvec4 _color; 
	OPfontAlign _align;

	OPvector* currNodes;
	bool isBuilt;
	HashMap* builtNodes;
	OPmeshPacker meshPacker;
} OPfontManager;




//-----------------------------------------------------------------------------
//   _____ _       _           _     
//  / ____| |     | |         | |    
// | |  __| | ___ | |__   __ _| |___ 
// | | |_ | |/ _ \| '_ \ / _` | / __|
// | |__| | | (_) | |_) | (_| | \__ \
//  \_____|_|\___/|_.__/ \__,_|_|___/
extern OPfontManager* OPRENDER_CURR_FONT_MANAGER;
extern OPeffect* OPRENDER_CURR_FONT_EFFECT;


//-----------------------------------------------------------------------------
// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
OPfontManager* OPfontManagerCreate(OPfont* font);
OPfontManager* OPfontManagerSetup(i8* font, i8** text, ui16 count);

void OPfontManagerBind(OPfontManager* manager);
void OPfontManagerAddText(const i8* text);
void OPfontManagerBuild(); 

void OPfontManagerSetColor4(OPfontManager* manager, OPvec4 color);
void OPfontManagerSetRGBA(OPfontManager* manager, f32 r, f32 g, f32 b, f32 a);
void OPfontManagerSetAlign(OPfontManager* manager, OPfontAlign align);

void OPfontEffectBind(OPeffect* effect);

void OPrenderTextXY(const i8* text, f32 x, f32 y);
void OPrenderTextXYAlign(const i8* text, f32 x, f32 y, OPfontAlign align);
void OPrenderTextRGBXY(const i8* text, f32 r, f32 g, f32 b, f32 x, f32 y);
void OPrenderTextRGBXYAlign(const i8* text, f32 r, f32 g, f32 b, f32 x, f32 y, OPfontAlign align);
void OPrenderTextRGBAXY(const i8* text, f32 r, f32 g, f32 b, f32 a, f32 x, f32 y);
void OPrenderTextRGBAXYAlign(const i8* text, f32 r, f32 g, f32 b, f32 a, f32 x, f32 y, OPfontAlign align);
void OPrenderTextVec2(const i8* text, OPvec2 pos);
void OPrenderTextVec2Align(const i8* text, OPvec2 pos, OPfontAlign align);
void OPrenderTextColor3Vec2(const i8* text, OPvec3 color, OPvec2 pos);
void OPrenderTextColor3Vec2Align(const i8* text, OPvec3 color, OPvec2 pos, OPfontAlign align);
void OPrenderTextColor4Vec2(const i8* text, OPvec4 color, OPvec2 pos);
void OPrenderTextColor4Vec2Align(const i8* text, OPvec4 color, OPvec2 pos, OPfontAlign align);
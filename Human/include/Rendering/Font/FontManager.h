#ifndef OPENGINE_HUMAN_RENDERER_FONT_MANAGER
#define OPENGINE_HUMAN_RENDERER_FONT_MANAGER

#include "./Human/include/Rendering/Font/Font.h"
#include "./Human/include/Rendering/Effect.h"
#include "./Human/include/Rendering/MeshPacked.h"
#include "./Math/include/Vectors.h"
#include "./Data/include/HashMap.h"

//-----------------------------------------------------------------------------
//   _____ _                   _       
//  / ____| |                 | |      
// | (___ | |_ _ __ _   _  ___| |_ ___ 
//  \___ \| __| '__| | | |/ __| __/ __|
//  ____) | |_| |  | |_| | (__| |_\__ \
// |_____/ \__|_|   \__,_|\___|\__|___/
typedef struct {
	OPchar* Text;
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
	OPhashMap* builtNodes;
	OPmeshPacker meshPacker;
	OPint pixelated;
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
OPfontManager* OPfontManagerSetup(OPchar* font, OPchar** text, ui16 count);

void OPfontManagerBind(OPfontManager* manager);
void OPfontManagerAddText(const OPchar* text);
void OPfontManagerBuild(); 
void OPfontManagerDestroy(OPfontManager* font);

void OPfontManagerSetColor4(OPfontManager* manager, OPvec4 color);
void OPfontManagerSetRGBA(OPfontManager* manager, f32 r, f32 g, f32 b, f32 a);
void OPfontManagerSetAlign(OPfontManager* manager, OPfontAlign align);

void OPfontEffectBind(OPeffect* effect);

void OPrenderTextXY(const OPchar* text, f32 x, f32 y);
void OPrenderTextXYAlign(const OPchar* text, f32 x, f32 y, OPfontAlign align);
void OPrenderTextRGBXY(const OPchar* text, f32 r, f32 g, f32 b, f32 x, f32 y);
void OPrenderTextRGBXYAlign(const OPchar* text, f32 r, f32 g, f32 b, f32 x, f32 y, OPfontAlign align);
void OPrenderTextRGBAXY(const OPchar* text, f32 r, f32 g, f32 b, f32 a, f32 x, f32 y);
void OPrenderTextRGBAXYAlign(const OPchar* text, f32 r, f32 g, f32 b, f32 a, f32 x, f32 y, OPfontAlign align);
void OPrenderTextVec2(const OPchar* text, OPvec2 pos);
void OPrenderTextVec2Align(const OPchar* text, OPvec2 pos, OPfontAlign align);
void OPrenderTextColor3Vec2(const OPchar* text, OPvec3 color, OPvec2 pos);
void OPrenderTextColor3Vec2Align(const OPchar* text, OPvec3 color, OPvec2 pos, OPfontAlign align);
void OPrenderTextColor4Vec2(const OPchar* text, OPvec4 color, OPvec2 pos);
void OPrenderTextColor4Vec2Align(const OPchar* text, OPvec4 color, OPvec2 pos, OPfontAlign align);
void OPrenderTextMat4(const OPchar* text, OPmat4* world);
void OPrenderTextColor4Mat4(const OPchar* text, OPvec4 color, OPmat4* world);

#endif
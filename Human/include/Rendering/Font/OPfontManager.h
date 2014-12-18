#ifndef OPENGINE_HUMAN_RENDERER_FONT_MANAGER
#define OPENGINE_HUMAN_RENDERER_FONT_MANAGER

#include "./Human/include/Rendering/Font/OPfont.h"
#include "./Human/include/Rendering/OPeffect.h"
#include "./Math/include/Vectors.h"
#include "./Data/include/OPhashMap.h"

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
OPfontManager* OPfontManagerSetup(const OPchar* font, const OPchar** text, ui16 count);

void OPfontManagerAddText(const OPchar* text);
void OPfontManagerBuild(); 
void OPfontManagerDestroy(OPfontManager* font);

void OPfontManagerSetColor(OPfontManager* manager, f32 r, f32 g, f32 b, f32 a);
void OPfontManagerSetColor(f32 r, f32 g, f32 b, f32 a);


// TODO: Refactor
inline void OPfontEffectBind(OPeffect* effect) {
	OPRENDER_CURR_FONT_EFFECT = effect;
}
inline void OPfontManagerBind(OPfontManager* manager) {
	OPRENDER_CURR_FONT_MANAGER = manager;
}
inline void OPfontManagerSetAlign(OPfontManager* manager, OPfontAlign align) {
	manager->_align = align;
}
inline void OPfontManagerSetAlign(OPfontAlign align) {
	OPRENDER_CURR_FONT_MANAGER->_align = align;
}
inline void OPfontManagerSetColor(OPfontManager* manager, OPvec3 color) {
	OPfontManagerSetColor(manager, color.x, color.y, color.z, 1.0);
}
inline void OPfontManagerSetColor(OPfontManager* manager, OPvec4 color) {
	OPfontManagerSetColor(manager, color.x, color.y, color.z, color.w);
}
inline void OPfontManagerSetColor(OPfontManager* manager, f32 r, f32 g, f32 b) {
	OPfontManagerSetColor(manager, r, g, b, 1.0);
}
inline void OPfontManagerSetColor(OPvec3 color) {
	OPfontManagerSetColor(OPRENDER_CURR_FONT_MANAGER, color.x, color.y, color.z, 1.0);
}
inline void OPfontManagerSetColor(OPvec4 color) {
	OPfontManagerSetColor(OPRENDER_CURR_FONT_MANAGER, color.x, color.y, color.z, color.w);
}
inline void OPfontManagerSetColor(f32 r, f32 g, f32 b) {
	OPfontManagerSetColor(OPRENDER_CURR_FONT_MANAGER, r, g, b, 1.0);
}
inline void OPfontManagerSetColor(f32 r, f32 g, f32 b, f32 a) {
	OPfontManagerSetColor(OPRENDER_CURR_FONT_MANAGER, r, g, b, a);
}

#endif
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
	OPfontUserTextNode dummyMesh;
	OPfont* _font;
	OPvec4 _color; 
	OPfontAlign _align;

	OPvector* currNodes;
	bool isBuilt;
	OPhashMap* builtNodes;
	OPmeshPacker meshPacker;
	OPint pixelated;
	OPmat4 proj;
	OPfloat scale;
} OPfontManager;




//-----------------------------------------------------------------------------
//   _____ _       _           _     
//  / ____| |     | |         | |    
// | |  __| | ___ | |__   __ _| |___ 
// | | |_ | |/ _ \| '_ \ / _` | / __|
// | |__| | | (_) | |_) | (_| | \__ \
//  \_____|_|\___/|_.__/ \__,_|_|___/

extern OPfontManager* OPFONTMANAGER_ACTIVE;
extern OPeffect* OPFONTMANAGER_EFFECT_ACTIVE;
extern OPeffect* OPFONTMANAGER_EFFECT2D_ACTIVE;


//-----------------------------------------------------------------------------
// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/

OPfontManager* OPfontManagerCreate(OPfont* font);
OPfontManager* OPfontManagerSetup(const OPchar* font, const OPchar** text, ui16 count, OPfloat scale);

void OPfontManagerAddText(const OPchar* text);
void OPfontManagerBuild(); 
void OPfontManagerDestroy(OPfontManager* font);

void OPfontManagerSetColor(OPfontManager* manager, f32 r, f32 g, f32 b, f32 a);
void OPfontManagerSetColor(f32 r, f32 g, f32 b, f32 a);


// TODO: Refactor
inline void OPfontEffectBind(OPeffect* effect) {
	OPFONTMANAGER_EFFECT_ACTIVE = effect;
}
inline void OPfontManagerBind(OPfontManager* manager) {
	OPFONTMANAGER_ACTIVE = manager;
}
inline void OPfontManagerSetAlign(OPfontManager* manager, OPfontAlign align) {
	manager->_align = align;
}
inline void OPfontManagerSetAlign(OPfontAlign align) {
	OPFONTMANAGER_ACTIVE->_align = align;
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
	OPfontManagerSetColor(OPFONTMANAGER_ACTIVE, color.x, color.y, color.z, 1.0);
}
inline void OPfontManagerSetColor(OPvec4 color) {
	OPfontManagerSetColor(OPFONTMANAGER_ACTIVE, color.x, color.y, color.z, color.w);
}
inline void OPfontManagerSetColor(f32 r, f32 g, f32 b) {
	OPfontManagerSetColor(OPFONTMANAGER_ACTIVE, r, g, b, 1.0);
}
inline void OPfontManagerSetColor(f32 r, f32 g, f32 b, f32 a) {
	OPfontManagerSetColor(OPFONTMANAGER_ACTIVE, r, g, b, a);
}

#endif
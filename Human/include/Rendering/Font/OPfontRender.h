#ifndef OPENGINE_HUMAN_RENDERER_FONT_RENDERTEXT
#define OPENGINE_HUMAN_RENDERER_FONT_RENDERTEXT

#include "./Human/include/Rendering/Font/OPfontManager.h"
#include "./Core/include/Assert.h"

void OPfontRenderBegin(OPfontManager* fontManager);
void OPfontRenderEnd();

void OPfontRender(const OPchar* text, OPmat4* world);
void OPfontRender(const OPchar* text, OPmat4* world, ui8 useJustWorld);
void OPfontRenderFill(const OPchar* text, OPmat4* world, f32 maxWidth);

inline void OPfontRender(const OPchar* text, OPvec2 pos) {
	OPmat4 world = OPmat4Translate(pos.x, pos.y, 0);
	OPfontRender(text, &world);
}
inline void OPfontRender(const OPchar* text, OPvec2 pos, f32 maxWidth) {
	OPmat4 world = OPmat4Translate(pos.x, pos.y, 0);
	OPfontRenderFill(text, &world, maxWidth);
}

inline void OPfontColor(OPvec4 color) {
	OPFONTMANAGER_EFFECT_ACTIVE->Bind();
	OPFONTMANAGER_EFFECT_ACTIVE->Set("uColor", &color);
}

#endif

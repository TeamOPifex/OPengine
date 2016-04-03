#ifndef OPENGINE_HUMAN_RENDERER_FONT_RENDERTEXT
#define OPENGINE_HUMAN_RENDERER_FONT_RENDERTEXT

#include "./Human/include/Rendering/Font/OPfontManager.h"
#include "./Core/include/Assert.h"

void OPfontRenderBegin(OPfontManager* fontManager);
void OPfontRenderEnd();

void OPfontRender(const OPchar* text, OPmat4* world);

inline void OPfontRender(const OPchar* text, OPvec2 pos) {
	OPmat4 world = OPmat4Translate(pos.x, pos.y, 0);
	OPfontRender(text, &world);
}

inline void OPfontColor(OPvec4 color) {
	OPmeshBind(&OPFONTMANAGER_ACTIVE->dummyMesh.mesh);
	OPeffectBind(OPFONTMANAGER_EFFECT_ACTIVE);
	OPeffectParamVec4("uColor", &color);
}

#endif

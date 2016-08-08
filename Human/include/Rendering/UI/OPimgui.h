#ifndef OPENGINE_HUMAN_RENDERING_UI_IMGUI
#define OPENGINE_HUMAN_RENDERING_UI_IMGUI

#include "./Human/include/Rendering/Font/OPfontManager.h"
#include "./Math/include/OPvec4.h"

struct OPimguiVertex {
	OPvec3 Position;
	OPvec4 Color;
};
typedef struct OPimguiVertex OPimguiVertex;

struct OPimgui {
	OPvertexBuffer buffer;
	OPmat4 proj;

	OPvec4 primaryColor;
	OPvec4 secondaryColor;
	OPvec4 hoverColor;
	OPvec4 borderColor;

	OPvec4 padding;
	OPvec4 margin;

	OPuint borderSize;

	OPfontManager* fontManager;
	OPeffect* effect;
};
typedef struct OPimgui OPimgui;

extern OPimgui* OPIMGUI_ACTIVE;


OPimgui* OPimguiCreate(OPeffect* effect, OPfontManager* fontManager);

void OPimguiBegin();
void OPimguiEnd();

OPint OPimguiRadio(OPvec2 pos, OPvec2 size, OPvec4 color, OPvec4 selected);
OPint OPimguiCheckbox( OPint state, OPvec2 pos,	OPvec2 size, OPvec4 color, OPvec4 selected, OPvec4 hover);
OPint OPimguiButton(OPvec2 pos, const OPchar* text, OPvec4 color, OPvec4 selected, OPvec4 hover);

OPint OPimguiTextbox(OPvec2 pos, const OPchar* text, const OPchar* placeholder, OPint active, OPint showCursor);
void OPimguiLabel(OPvec2 pos, const OPchar* text);
void OPimguiLabel(OPvec2 pos, OPvec2 size, const OPchar* text, OPvec4 bg, OPvec4 color, OPint fill);

inline void OPimguiLabel(OPvec2 pos, OPvec2 size, const OPchar* text, OPvec4 color) {
	OPimguiLabel(pos, size, text, OPvec4Create(0), color, 0);
}

inline OPint OPimguiCheckbox(OPint state, OPvec2 pos, OPvec2 size) {
	return OPimguiCheckbox(state, pos, size, OPIMGUI_ACTIVE->secondaryColor, OPIMGUI_ACTIVE->primaryColor, OPIMGUI_ACTIVE->hoverColor);
}

inline OPint OPimguiButton(OPvec2 pos, const OPchar* text) {
	return OPimguiButton(pos, text, OPIMGUI_ACTIVE->secondaryColor, OPIMGUI_ACTIVE->primaryColor, OPIMGUI_ACTIVE->hoverColor);
}

inline void OPimguiBind(OPimgui* imgui) {
	OPIMGUI_ACTIVE = imgui;
}

#endif

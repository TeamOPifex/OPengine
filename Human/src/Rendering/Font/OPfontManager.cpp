#include "./Human/include/Rendering/Font/OPfontManager.h"
#include "./Human/include/Rendering/OPrender.h"
#include "./Human/include/Systems/OPfontSystem.h"
#include "./Human/include/Rendering/OPmeshPacked.h"
#include "./Data/include/OPcman.h"
#include "./Core/include/Assert.h"

OPfontManager* OPRENDER_CURR_FONT_MANAGER = NULL;
OPeffect* OPRENDER_CURR_FONT_EFFECT = NULL;

OPfontManager* OPfontManagerCreate(OPfont* font) {
	OPfontManager* temp = (OPfontManager*)OPallocZero(sizeof(OPfontManager));
	temp->_font = font;
	temp->_color = OPvec4Create(0.0f, 0.0f, 0.0f, 1.0f);
	temp->_align = OPFONT_ALIGN_LEFT;

	temp->currNodes = OPvectorCreate(sizeof(OPfontTextNode));
	temp->isBuilt = false;
	temp->builtNodes = OPhashMapCreate(16);
	temp->meshPacker = OPmeshPackerCreate();
	return temp;
}

OPfontManager* OPfontManagerSetup(const OPchar* font, const OPchar** text, ui16 count) {
	OPsystemsLoadFontEffect();
	OPcmanLoad(font);
	OPfont* _font = (OPfont*)OPcmanGet(font);
	OPfontManager* manager = OPfontManagerCreate(_font);
	OPfontManagerBind(manager);
	for (ui16 i = 0; i < count; i++) {
		OPfontManagerAddText(text[i]);
	}
	OPfontManagerBuild();
	return manager;
}

void OPfontManagerDestroy(OPfontManager* font) {
	OPRENDER_CURR_FONT_MANAGER = NULL;
	OPvectorDestroy(font->currNodes);
	OPhashMapDestroy(font->builtNodes);
	OPfree(font);
}

void OPfontManagerSetColor(OPfontManager* manager, f32 r, f32 g, f32 b, f32 a) {
	manager->_color.x = r;
	manager->_color.y = g;
	manager->_color.z = b;
	manager->_color.w = a;
}

void OPfontManagerAddText(const OPchar* text) {
	ASSERT(OPRENDER_CURR_FONT_MANAGER != NULL, "A Font Manager has not been bound yet");
	OPmeshPackerBind(&OPRENDER_CURR_FONT_MANAGER->meshPacker);
	OPfontBuiltTextNode* node = (OPfontBuiltTextNode*)OPalloc(sizeof(OPfontBuiltTextNode));
	*node = OPfontCreatePackedText(OPRENDER_CURR_FONT_MANAGER->_font, text);
	OPhashMapPut(OPRENDER_CURR_FONT_MANAGER->builtNodes, text, node);
}

void OPfontManagerBuild() {
	ASSERT(OPRENDER_CURR_FONT_MANAGER != NULL, "A Font Manager has not been bound yet");
	OPmeshPackerBind(&OPRENDER_CURR_FONT_MANAGER->meshPacker);
	OPmeshPackerBuild();
	OPRENDER_CURR_FONT_MANAGER->isBuilt = true;
}

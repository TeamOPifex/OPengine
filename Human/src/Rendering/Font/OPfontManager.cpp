#include <include/Rendering/Font/OPfontManager.h>
#include <include/Rendering/Font/OPfont.h>
#include "./Human/include/Rendering/Font/OPfontManager.h"
#include "./Human/include/Rendering/OPrender.h"
#include "./Human/include/Systems/OPfontSystem.h"
#include "./Human/include/Rendering/OPmeshPacked.h"
#include "./Data/include/OPcman.h"
#include "./Core/include/Assert.h"

OPfontManager* OPFONTMANAGER_ACTIVE = NULL;
OPeffect* OPFONTMANAGER_EFFECT_ACTIVE = NULL;
OPeffect* OPFONTMANAGER_EFFECT2D_ACTIVE = NULL;

OPfontManager* OPfontManagerCreate(OPfont* font) {
	OPfontManager* temp = (OPfontManager*)OPallocZero(sizeof(OPfontManager));
	temp->_font = font;
	temp->_color = OPvec4Create(0.0f, 0.0f, 0.0f, 1.0f);
	temp->_align = OPFONT_ALIGN_LEFT;

	temp->scale = 1.0f;
	temp->currNodes = OPvectorCreate(sizeof(OPfontTextNode));
	temp->isBuilt = false;
	temp->builtNodes = OPhashMapCreate(16);
	temp->meshPacker.Init();
	temp->proj = OPmat4Ortho(0, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, 0, -1, 1);
	temp->dummyMesh = OPfontCreateUserText(temp->_font, "", temp->scale);

	return temp;
}

OPfontManager* OPfontManagerSetup(const OPchar* font, const OPchar** text, ui16 count) {
	OPfontSystemLoadEffects();
	OPfont* _font = (OPfont*)OPcmanLoadGet(font);
	OPfontManager* manager = OPfontManagerCreate(_font);
	manager->scale = 1.0;
	manager->_color = OPvec4Create(1,1,1,1);
	OPfontManagerBind(manager);
	for (ui16 i = 0; i < count; i++) {
		OPfontManagerAddText(text[i]);
	}
	OPfontManagerBuild();
	OPFONTMANAGER_ACTIVE = NULL;

	return manager;
}

void OPfontManagerDestroy(OPfontManager* font) {
	OPFONTMANAGER_ACTIVE = NULL;
	OPvectorDestroy(font->currNodes);
	OPfree(font->currNodes);
	OPhashMapDestroy(font->builtNodes);
	OPfree(font->builtNodes);
	OPfree(font);
}

void OPfontManagerSetColor(OPfontManager* manager, f32 r, f32 g, f32 b, f32 a) {
	manager->_color.x = r;
	manager->_color.y = g;
	manager->_color.z = b;
	manager->_color.w = a;
}

void OPfontManagerAddText(const OPchar* text) {
	ASSERT(OPFONTMANAGER_ACTIVE != NULL, "A Font Manager has not been bound yet");
	OPFONTMANAGER_ACTIVE->meshPacker.Bind();
	OPfontBuiltTextNode* node = (OPfontBuiltTextNode*)OPalloc(sizeof(OPfontBuiltTextNode));
	*node = OPfontCreatePackedText(OPFONTMANAGER_ACTIVE->_font, text, OPFONTMANAGER_ACTIVE->scale);
	OPhashMapPut(OPFONTMANAGER_ACTIVE->builtNodes, text, node);
}

void OPfontManagerBuild() {
	ASSERT(OPFONTMANAGER_ACTIVE != NULL, "A Font Manager has not been bound yet");
	OPFONTMANAGER_ACTIVE->meshPacker.Bind();
	OPFONTMANAGER_ACTIVE->meshPacker.Build();
	OPFONTMANAGER_ACTIVE->isBuilt = true;
}

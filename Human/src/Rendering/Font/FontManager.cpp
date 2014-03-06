#include "./Human/include/Rendering/Font/FontManager.h"
#include "./Human/include/Rendering/Renderer.h"
#include "./Human/include/Systems/FontSystem.h"
#include "./Data/include/ContentManager.h"
#include "./Core/include/Assert.h"

OPfontManager* OPRENDER_CURR_FONT_MANAGER = NULL;
OPeffect* OPRENDER_CURR_FONT_EFFECT = NULL;

OPfontManager* OPfontManagerCreate(OPfont* font) {
	OPfontManager* temp = (OPfontManager*)OPalloc(sizeof(OPfontManager));
	temp->_font = font;
	temp->_color = OPvec4Create(0.0f, 0.0f, 0.0f, 1.0f);
	temp->_align = OPFONT_ALIGN_LEFT;

	temp->currNodes = OPvectorCreate(sizeof(OPfontTextNode));
	temp->isBuilt = false;
	temp->builtNodes = OPhashMapCreate(16);
	temp->meshPacker = OPmeshPackerCreate();
	return temp;
}

OPfontManager* OPfontManagerSetup(OPchar* font, OPchar** text, ui16 count) {
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

void OPfontManagerSetColor4(OPfontManager* manager, OPvec4 color) {
	manager->_color = color;
}

void OPfontManagerSetRGBA(OPfontManager* manager, f32 r, f32 g, f32 b, f32 a) {
	manager->_color.x = r;
	manager->_color.y = g;
	manager->_color.z = b;
	manager->_color.w = a;
}

void OPfontManagerSetAlign(OPfontManager* manager, OPfontAlign align) {
	manager->_align = align;
}

void OPfontManagerBind(OPfontManager* manager) {
	OPRENDER_CURR_FONT_MANAGER = manager;
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

void OPfontEffectBind(OPeffect* effect) {
	OPRENDER_CURR_FONT_EFFECT = effect;
}

void OPrenderTextRGBAXYAlign(const OPchar* text, f32 r, f32 g, f32 b, f32 a, f32 x, f32 y, OPfontAlign align) {
	OPrenderTextColor4Vec2Align(text, OPvec4Create(r, g, b, a), OPvec2Create(x, y), align);
}

void OPrenderTextXY(const OPchar* text, f32 x, f32 y) {
	ASSERT(OPRENDER_CURR_FONT_MANAGER != NULL, "A Font Manager has not been bound yet");
	//OPvec4 col = OPRENDER_CURR_FONT_MANAGER->_color;
	//OPvec2 pos = OPvec2Create(x, y);
	//OPfontAlign a = OPRENDER_CURR_FONT_MANAGER->_align;
	OPrenderTextColor4Vec2Align(text, OPRENDER_CURR_FONT_MANAGER->_color, OPvec2Create(x, y), OPRENDER_CURR_FONT_MANAGER->_align);
}

void OPrenderTextXYAlign(const OPchar* text, f32 x, f32 y, OPfontAlign align) {
	ASSERT(OPRENDER_CURR_FONT_MANAGER != NULL, "A Font Manager has not been bound yet");
	OPrenderTextColor4Vec2Align(text, OPRENDER_CURR_FONT_MANAGER->_color, OPvec2Create(x, y), align);
}

void OPrenderTextRGBXY(const OPchar* text, f32 r, f32 g, f32 b, f32 x, f32 y) {
	ASSERT(OPRENDER_CURR_FONT_MANAGER != NULL, "A Font Manager has not been bound yet");
	OPrenderTextColor4Vec2Align(text, OPvec4Create(r, g, b, 1.0f), OPvec2Create(x, y), OPRENDER_CURR_FONT_MANAGER->_align);
}

void OPrenderTextRGBAXY(const OPchar* text, f32 r, f32 g, f32 b, f32 a, f32 x, f32 y) {
	ASSERT(OPRENDER_CURR_FONT_MANAGER != NULL, "A Font Manager has not been bound yet");
	OPrenderTextColor4Vec2Align(text, OPvec4Create(r,g,b,a), OPvec2Create(x,y), OPRENDER_CURR_FONT_MANAGER->_align);
}
void OPrenderTextVec2(const OPchar* text, OPvec2 pos) {
	ASSERT(OPRENDER_CURR_FONT_MANAGER != NULL, "A Font Manager has not been bound yet");
	OPrenderTextColor4Vec2Align(text, OPRENDER_CURR_FONT_MANAGER->_color, pos, OPRENDER_CURR_FONT_MANAGER->_align);
}

void OPrenderTextVec2Align(const OPchar* text, OPvec2 pos, OPfontAlign align) {
	ASSERT(OPRENDER_CURR_FONT_MANAGER != NULL, "A Font Manager has not been bound yet");
	OPrenderTextColor4Vec2Align(text, OPRENDER_CURR_FONT_MANAGER->_color, pos, align);
}

void OPrenderTextColor3Vec2(const OPchar* text, OPvec3 color, OPvec2 pos) {
	OPrenderTextColor4Vec2Align(text, OPvec4CreateFromVec3(color, 1.0f), pos, OPRENDER_CURR_FONT_MANAGER->_align);
}

void OPrenderTextColor3Vec2Align(const OPchar* text, OPvec3 color, OPvec2 pos, OPfontAlign align) {
	OPrenderTextColor4Vec2Align(text, OPvec4CreateFromVec3(color, 1.0f), pos, align);
}

void OPrenderTextColor4Vec2(const OPchar* text, OPvec4 color, OPvec2 pos) {
	ASSERT(OPRENDER_CURR_FONT_MANAGER != NULL, "A Font Manager has not been bound yet");
	OPrenderTextColor4Vec2Align(text, color, pos, OPRENDER_CURR_FONT_MANAGER->_align);
}

void OPrenderTextColor4Vec2Align(const OPchar* text, OPvec4 color, OPvec2 pos, OPfontAlign align) {
	ASSERT(OPRENDER_CURR_FONT_EFFECT != NULL, "A Font Effect has not been bound yet");
	ASSERT(OPRENDER_CURR_FONT_MANAGER != NULL, "A Font Manager has not been bound yet");
	ASSERT(OPRENDER_CURR_FONT_MANAGER->builtNodes != NULL, "The bound Font Manager Hashmap hasn't been created yet");

	OPrenderDepth(0);

	int tryHashMap = OPRENDER_CURR_FONT_MANAGER->isBuilt;
	OPfontBuiltTextNode* node = NULL;
	if (tryHashMap) {
		//ASSERT(OPhashMapExists(OPRENDER_CURR_FONT_MANAGER->builtNodes, text), "No node exists for text in this Font Manager");
		OPhashMapGet(OPRENDER_CURR_FONT_MANAGER->builtNodes, text, (void**)&node);
	}

	OPrenderBindEffect(OPRENDER_CURR_FONT_EFFECT);
	OPtextureClearActive();
	ui32 textureHandle = OPtextureBind(OPRENDER_CURR_FONT_MANAGER->_font->texture);
	OPrenderParami("uColorTexture", textureHandle);
	OPrenderParamVec4("uColor", 1, &color);
	OPmat4 world;
	OPfloat scale = OPrenderWidth / 2.0f;

	OPmat4scl(&world, OPrenderGetWidthAspectRatio() / scale, OPrenderGetHeightAspectRatio() / scale, 1.0f / scale);
	OPmat4translate(&world, pos.x, pos.y, 0.0f);
	OPrenderParamMat4v("uWorld", 1, &world);

	if (node == NULL || !OPRENDER_CURR_FONT_MANAGER->isBuilt) {

		OPfontUserTextNode node = OPfontCreateUserText(OPRENDER_CURR_FONT_MANAGER->_font, text);
		OPrenderBindMesh(&node.mesh);
		OPrenderBindEffect(OPRENDER_CURR_FONT_EFFECT);

		// Scale it down to half the screen width to get it into pixel values
		switch (align) {
		case OPFONT_ALIGN_LEFT:
			OPmat4identity(&world);
			break;
		case OPFONT_ALIGN_CENTER:
			OPmat4buildTranslate(&world, -(node.Width / 2.0f), 0, 0.0f);
			break;
		case OPFONT_ALIGN_RIGHT:
			OPmat4buildTranslate(&world, -node.Width, 0, 0.0f);
			break;
		}

		OPmat4scl(&world, OPrenderGetWidthAspectRatio() / scale, OPrenderGetHeightAspectRatio() / scale, 1.0f / scale);
		OPmat4translate(&world, pos.x, pos.y, 0.0f);
		OPrenderParamMat4v("uWorld", 1, &world);

		OPrenderMesh();

		OPrenderDestroyMesh(&node.mesh);
	}
	else 
	{
		ASSERT(OPRENDER_CURR_FONT_MANAGER->isBuilt, "The bound Font Manager has not been built yet");
		ASSERT(node->packedMesh != NULL, "The Packed Mesh is invalid");

		// Don't bother trying to render text with a width of 0
		if (node->Width == 0) return;

		OPmeshPackerBind(&OPRENDER_CURR_FONT_MANAGER->meshPacker);
		OPrenderBindEffect(OPRENDER_CURR_FONT_EFFECT);


		// Scale it down to half the screen width to get it into pixel values
		switch (align) {
		case OPFONT_ALIGN_LEFT:
			OPmat4identity(&world);
			break;
		case OPFONT_ALIGN_CENTER:
			OPmat4buildTranslate(&world, -(node->Width / 2.0f), 0, 0.0f);
			break;
		case OPFONT_ALIGN_RIGHT:
			OPmat4buildTranslate(&world, -node->Width, 0, 0.0f);
			break;
		}

		OPmat4scl(&world, OPrenderGetWidthAspectRatio() / scale, OPrenderGetHeightAspectRatio() / scale, 1.0f / scale);
		OPmat4translate(&world, pos.x, pos.y, 0.0f);
		OPrenderParamMat4v("uWorld", 1, &world);

		OPrenderMeshPacked(node->packedMesh);
	}
}

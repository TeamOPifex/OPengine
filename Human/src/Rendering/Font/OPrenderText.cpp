#include "./Human/include/Rendering/Font/OPfontRender.h"
#include "./Human/include/Rendering/OPrender.h"

void OPfontRenderBegin(OPfontManager* fontManager) {
	ASSERT(fontManager != NULL, "A font manager must be used to begin");
	ASSERT(OPFONTMANAGER_EFFECT_ACTIVE != NULL, "A font effect must be used to begin");
	ASSERT(OPFONTMANAGER_ACTIVE == NULL, "A font manager has already been activated. Did you call OPfontRenderEnd?");
	
	OPFONTMANAGER_ACTIVE = fontManager;

	OPrenderDepth(0);
	fontManager->dummyMesh.mesh.Bind();
	OPeffectBind(OPFONTMANAGER_EFFECT_ACTIVE);
	OPtextureClearActive();
	ui32 textureHandle = OPtextureBind(OPFONTMANAGER_ACTIVE->_font->texture);
	OPeffectParami("uColorTexture", textureHandle);
	OPeffectParamVec4("uColor", &OPFONTMANAGER_ACTIVE->_color);
	OPeffectParamMat4("uProj", &OPFONTMANAGER_ACTIVE->proj);
	if (OPFONTMANAGER_ACTIVE->pixelated) OPtexturePixelate();
}

void OPfontRenderEnd() {
	OPFONTMANAGER_ACTIVE = NULL;
	OPrenderDepth(1);
}

void OPfontRender(OPfontUserTextNode* node, OPmat4* world) {
	node->mesh.Bind();
	OPeffectBind(OPFONTMANAGER_EFFECT_ACTIVE);
  	OPeffectParamMat4v("uWorld", 1, world);
	OPmeshRender();
}

void OPfontRender(OPfontBuiltTextNode* node, OPmat4* world) {
	OPmeshPackerBind(&OPFONTMANAGER_ACTIVE->meshPacker);
	OPeffectBind(OPFONTMANAGER_EFFECT_ACTIVE);
 	OPeffectParamMat4v("uWorld", 1, world);
	OPmeshPackedRender(node->packedMesh);
}

void OPfontRenderSetAlign(OPmat4* world, OPfloat width, OPfontAlign align){
	switch (align) {
		case OPFONT_ALIGN_LEFT:
			*world = OPmat4Translate(0, 0, 0.0f);
			break;
		case OPFONT_ALIGN_CENTER:
			*world = OPmat4Translate(-(width / 2.0f), 0, 0.0f);
			break;
		case OPFONT_ALIGN_RIGHT:
			*world = OPmat4Translate(-width, 0, 0.0f);
			break;
	}
}

void OPfontRender(const OPchar* text, OPmat4* world) {
	ASSERT(OPFONTMANAGER_EFFECT_ACTIVE != NULL, "A Font Effect has not been bound yet");
	ASSERT(OPFONTMANAGER_ACTIVE != NULL, "A Font Manager has not been bound yet");
	ASSERT(OPFONTMANAGER_ACTIVE->builtNodes != NULL, "The bound Font Manager Hashmap hasn't been created yet");

	int tryHashMap = OPFONTMANAGER_ACTIVE->isBuilt;
	OPfontBuiltTextNode* node = NULL;
	if (tryHashMap) {
		OPhashMapGet(OPFONTMANAGER_ACTIVE->builtNodes, text, (void**)&node);
	}

	OPmat4 aligned;

	if (node == NULL || !OPFONTMANAGER_ACTIVE->isBuilt) {
		OPfontUserTextNode textNode = OPfontCreateUserText(OPFONTMANAGER_ACTIVE->_font, text, 1.0);
		OPfontRenderSetAlign(&aligned, textNode.Width, OPFONTMANAGER_ACTIVE->_align);
		OPmat4 temp = (*world) * aligned * OPmat4Scl(OPFONTMANAGER_ACTIVE->scale);
		OPfontRender(&textNode, &temp);
		OPmeshDestroy(&textNode.mesh);
	}
	else {
		OPfontRenderSetAlign(&aligned, node->Width, OPFONTMANAGER_ACTIVE->_align);
		OPmat4 temp = (*world) * aligned * OPmat4Scl(OPFONTMANAGER_ACTIVE->scale);
		OPfontRender(node, &temp);
	}
}

void OPfontRender(const OPchar* text, OPmat4* world, ui8 useJustWorld) {
	ASSERT(OPFONTMANAGER_EFFECT_ACTIVE != NULL, "A Font Effect has not been bound yet");
	ASSERT(OPFONTMANAGER_ACTIVE != NULL, "A Font Manager has not been bound yet");
	ASSERT(OPFONTMANAGER_ACTIVE->builtNodes != NULL, "The bound Font Manager Hashmap hasn't been created yet");

	int tryHashMap = OPFONTMANAGER_ACTIVE->isBuilt;
	OPfontBuiltTextNode* node = NULL;
	if (tryHashMap) {
		OPhashMapGet(OPFONTMANAGER_ACTIVE->builtNodes, text, (void**)&node);
	}

	OPmat4 aligned;

	if (node == NULL || !OPFONTMANAGER_ACTIVE->isBuilt) {
		OPfontUserTextNode textNode = OPfontCreateUserText(OPFONTMANAGER_ACTIVE->_font, text, 1.0);
		OPfontRender(&textNode, world);
		OPmeshDestroy(&textNode.mesh);
	}
	else {
		OPfontRender(node, world);
	}
}
#include "./Human/include/Rendering/Font/OPfontRender.h"
#include "./Human/include/Rendering/OPrender.h"

void OPfontRenderBegin(OPfontManager* fontManager) {
	ASSERT(fontManager != NULL, "A font manager must be used to begin");
	ASSERT(OPFONTMANAGER_EFFECT_ACTIVE != NULL, "A font effect must be used to begin");
	ASSERT(OPFONTMANAGER_ACTIVE == NULL, "A font manager has already been activated. Did you call OPfontRenderEnd?");
	
	OPFONTMANAGER_ACTIVE = fontManager;

	OPrenderDepth(0);
	OPeffectBind(OPFONTMANAGER_EFFECT_ACTIVE);
	OPtextureClearActive();
	ui32 textureHandle = OPtextureBind(OPFONTMANAGER_ACTIVE->_font->texture);
	OPeffectParami("uColorTexture", textureHandle);
	OPeffectParamVec4("uColor", &OPFONTMANAGER_ACTIVE->_color);
	OPeffectParamMat4("uProj", &OPFONTMANAGER_ACTIVE->proj);
	//OPmat4Log("proj", OPFONTMANAGER_ACTIVE->proj);
	if (OPFONTMANAGER_ACTIVE->pixelated) OPtexturePixelate();
}

void OPfontRenderEnd() {
	OPFONTMANAGER_ACTIVE = NULL;
	OPrenderDepth(1);
}

// void OPfontRenderSetParameters(OPvec4 color, OPmat4* world) {
// 	OPrenderDepth(0);
// 	OPeffectBind(OPFONTMANAGER_EFFECT_ACTIVE);
// 	OPtextureClearActive();
// 	ui32 textureHandle = OPtextureBind(OPFONTMANAGER_ACTIVE->_font->texture);
// 	OPeffectParami("uColorTexture", textureHandle);
// 	OPeffectParamVec4("uColor", &color);
// 	OPeffectParamMat4v("uWorld", 1, world);
// 	if (OPFONTMANAGER_ACTIVE->pixelated) OPtexturePixelate();
// }

// void OPfontRenderSet2DParameters(OPvec4 color, OPmat4* world) {
// 	OPrenderDepth(0);
// 	OPeffectBind(OPFONTMANAGER_EFFECT2D_ACTIVE);
// 	OPtextureClearActive();
// 	ui32 textureHandle = OPtextureBind(OPFONTMANAGER_ACTIVE->_font->texture);
// 	OPeffectParami("uColorTexture", textureHandle);
// 	OPeffectParamVec4("uColor", &color);
// 	OPeffectParamMat4v("uWorld", 1, world);
// 	OPeffectParamMat4v("uProj", 1, &OPFONTMANAGER_ACTIVE->proj);
// }

void OPfontRender(OPfontUserTextNode* node, OPmat4* world) {
	OPmeshBind(&node->mesh);
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

// void OPfontRender2D(OPfontUserTextNode* node, OPvec4 color, OPmat4* world) {
// 	OPmeshBind(&node->mesh);
// 	OPfontRenderSet2DParameters(color, world);
// 	OPmeshRender();
// }

// void OPfontRender2D(OPfontBuiltTextNode* node, OPvec4 color, OPmat4* world) {
// 	OPmeshPackerBind(&OPFONTMANAGER_ACTIVE->meshPacker);
// 	OPfontRenderSet2DParameters(color, world);
// 	if (OPFONTMANAGER_ACTIVE->pixelated) OPtexturePixelate();
// 	OPmeshPackedRender(node->packedMesh);
// }

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

// void OPfontRender2D(const OPchar* text, OPvec4 color, OPvec2 pos, OPfontAlign align) {
// 	OPmat4 world = OPmat4Translate(pos.x, pos.y, 0);
// 	OPfontRender2D(text, color, &world, align);
// }

// void OPfontRender(const OPchar* text, OPvec4 color, OPvec2 pos, OPfontAlign align) {
// 	OPmat4 world = OPmat4Translate(pos.x, pos.y, 0);
// 	OPfontRender(text, color, &world, align);
// }

// void OPfontRender(const OPchar* text, OPmat4* world) {
// 	ASSERT(OPFONTMANAGER_EFFECT_ACTIVE != NULL, "A Font Effect has not been bound yet");
// 	ASSERT(OPFONTMANAGER_ACTIVE != NULL, "A Font Manager has not been bound yet");
// 	ASSERT(OPFONTMANAGER_ACTIVE->builtNodes != NULL, "The bound Font Manager Hashmap hasn't been created yet");

// 	int tryHashMap = OPFONTMANAGER_ACTIVE->isBuilt;
// 	OPfontBuiltTextNode* node = NULL;
// 	if (tryHashMap) {
// 		OPhashMapGet(OPFONTMANAGER_ACTIVE->builtNodes, text, (void**)&node);
// 	}

// 	OPmat4 aligned;
// 	OPmat4 scaled;
// 	OPfloat scale = (OPRENDER_WIDTH / 2.0f) * OPRENDER_SCREEN_WIDTH_SCALE;

// 	if (node == NULL || !OPFONTMANAGER_ACTIVE->isBuilt) {
// 		OPfontUserTextNode textNode = OPfontCreateUserText(OPFONTMANAGER_ACTIVE->_font, text);

// 		OPfontRenderSetAlign(&aligned, textNode.Width, align);

// 		scaled = OPmat4Scl(
// 			OPrenderGetWidthAspectRatio() / scale, 
// 			OPrenderGetHeightAspectRatio() / scale, 
// 			1.0f);

// 		OPmat4 temp;
// 		OPmat4Mul(&temp, scaled, aligned);
// 		OPmat4Mul(&temp, *world, temp);

// 		OPfontRender(&textNode, color, &temp);
// 		OPmeshDestroy(&textNode.mesh);
// 	}
// 	else {
// 		OPfontRenderSetAlign(&aligned, node->Width, align);

// 		scaled = OPmat4Scl(
// 			OPrenderGetWidthAspectRatio() / scale,
// 			OPrenderGetHeightAspectRatio() / scale,
// 			1.0f);
// 		OPmat4 temp = scaled * aligned;
// 		temp = (*world) * temp;
// 		OPfontRender(node, color, &temp);
// 	}
// }


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
		OPfontUserTextNode textNode = OPfontCreateUserText(OPFONTMANAGER_ACTIVE->_font, text, OPFONTMANAGER_ACTIVE->scale);
		OPfontRenderSetAlign(&aligned, textNode.Width, OPFONTMANAGER_ACTIVE->_align);
		OPmat4 temp = (*world) * aligned;
		//OPmat4Log("font", temp);
		OPfontRender(&textNode, &temp);
		OPmeshDestroy(&textNode.mesh);
	}
	else {
		OPfontRenderSetAlign(&aligned, node->Width, OPFONTMANAGER_ACTIVE->_align);
		OPmat4 temp = (*world) * aligned;
		OPfontRender(node, &temp);
	}
}
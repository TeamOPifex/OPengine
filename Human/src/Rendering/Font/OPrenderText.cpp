#include "./Human/include/Rendering/Font/OPfontRender.h"
#include "./Human/include/Rendering/OPrender.h"

void OPfontRenderSetParameters(OPvec4 color, OPmat4* world) {
	OPrenderDepth(0);
	OPeffectBind(OPFONTMANAGER_EFFECT_ACTIVE);
	OPtextureClearActive();
	ui32 textureHandle = OPtextureBind(OPFONTMANAGER_ACTIVE->_font->texture);
	OPeffectParami("uColorTexture", textureHandle);
	OPeffectParamVec4("uColor", &color);
	OPeffectParamMat4v("uWorld", 1, world);
}

void OPfontRender(OPfontUserTextNode* node, OPvec4 color, OPmat4* world) {
	OPfontRenderSetParameters(color, world);
	OPmeshBind(&node->mesh);
	OPeffectBind(OPFONTMANAGER_EFFECT_ACTIVE);
	if (OPFONTMANAGER_ACTIVE->pixelated) OPtexturePixelate();
	OPmeshRender();
}

void OPfontRender(OPfontBuiltTextNode* node, OPvec4 color, OPmat4* world) {
	OPfontRenderSetParameters(color, world);
	OPmeshPackerBind(&OPFONTMANAGER_ACTIVE->meshPacker);
	OPeffectBind(OPFONTMANAGER_EFFECT_ACTIVE);
	if (OPFONTMANAGER_ACTIVE->pixelated) OPtexturePixelate();
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

void OPfontRender(const OPchar* text, OPvec4 color, OPvec2 pos, OPfontAlign align) {
	ASSERT(OPFONTMANAGER_EFFECT_ACTIVE != NULL, "A Font Effect has not been bound yet");
	ASSERT(OPFONTMANAGER_ACTIVE != NULL, "A Font Manager has not been bound yet");
	ASSERT(OPFONTMANAGER_ACTIVE->builtNodes != NULL, "The bound Font Manager Hashmap hasn't been created yet");

	int tryHashMap = OPFONTMANAGER_ACTIVE->isBuilt;
	OPfontBuiltTextNode* node = NULL;
	if (tryHashMap) {
		OPhashMapGet(OPFONTMANAGER_ACTIVE->builtNodes, text, (void**)&node);
	}

	OPmat4 world;
	OPfloat scale = (OPRENDER_WIDTH / 2.0f) * OPRENDER_SCREEN_WIDTH_SCALE;

	if (node == NULL || !OPFONTMANAGER_ACTIVE->isBuilt) {
		OPfontUserTextNode textNode = OPfontCreateUserText(OPFONTMANAGER_ACTIVE->_font, text);
		OPfontRenderSetAlign(&world, textNode.Width, align);
		OPmat4Scl(&world, OPrenderGetWidthAspectRatio() / scale, OPrenderGetHeightAspectRatio() / scale, 1.0f);
		OPmat4Translate(&world, pos.x, pos.y, 0.0f);
		OPfontRender(&textNode, color, &world);
		OPmeshDestroy(&textNode.mesh);
	} else {
		OPfontRenderSetAlign(&world, node->Width, align);
		OPmat4Scl(&world, OPRENDER_SCREEN_WIDTH_SCALE * OPrenderGetWidthAspectRatio() / scale, OPRENDER_SCREEN_HEIGHT_SCALE * OPrenderGetHeightAspectRatio() / scale, 1.0f);
		OPmat4Translate(&world, pos.x, pos.y, 0.0f);
		OPfontRender(node, color, &world);
	}
}

void OPfontRender(const OPchar* text, OPvec4 color, OPmat4* world) {
	ASSERT(OPFONTMANAGER_EFFECT_ACTIVE != NULL, "A Font Effect has not been bound yet");
	ASSERT(OPFONTMANAGER_ACTIVE != NULL, "A Font Manager has not been bound yet");
	ASSERT(OPFONTMANAGER_ACTIVE->builtNodes != NULL, "The bound Font Manager Hashmap hasn't been created yet");

	int tryHashMap = OPFONTMANAGER_ACTIVE->isBuilt;
	OPfontBuiltTextNode* node = NULL;
	if (tryHashMap) {
		OPhashMapGet(OPFONTMANAGER_ACTIVE->builtNodes, text, (void**)&node);
	}

	OPmat4 aligned;
	OPmat4 scaled;
	OPfloat scale = (OPRENDER_WIDTH / 2.0f) * OPRENDER_SCREEN_WIDTH_SCALE;

	if (node == NULL || !OPFONTMANAGER_ACTIVE->isBuilt) {
		OPfontUserTextNode textNode = OPfontCreateUserText(OPFONTMANAGER_ACTIVE->_font, text);

		OPfontRenderSetAlign(&aligned, textNode.Width, OPFONTMANAGER_ACTIVE->_align);

		scaled = OPmat4Scl(
			OPrenderGetWidthAspectRatio() / scale, 
			OPrenderGetHeightAspectRatio() / scale, 
			1.0f);

		OPmat4 temp;
		OPmat4Mul(&temp, scaled, aligned);
		OPmat4Mul(&temp, *world, temp);

		OPfontRender(&textNode, color, &temp);
		OPmeshDestroy(&textNode.mesh);
	}
	else {
		OPfontRenderSetAlign(&aligned, node->Width, OPFONTMANAGER_ACTIVE->_align);

		scaled = OPmat4Scl(
			OPrenderGetWidthAspectRatio() / scale,
			OPrenderGetHeightAspectRatio() / scale,
			1.0f);
		OPmat4 temp = scaled * aligned;
		temp = (*world) * temp;
		OPfontRender(node, color, &temp);
	}
}
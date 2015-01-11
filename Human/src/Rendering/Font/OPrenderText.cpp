#include "./Human/include/Rendering/Font/OPrenderText.h"
#include "./Human/include/Rendering/OPrender.h"

void OPrenderTextSetParameters(OPvec4 color, OPmat4* world) {
	OPrenderDepth(0);
	OPrenderBindEffect(OPRENDER_CURR_FONT_EFFECT);
	OPtextureClearActive();
	ui32 textureHandle = OPtextureBind(OPRENDER_CURR_FONT_MANAGER->_font->texture);
	OPrenderParami("uColorTexture", textureHandle);
	OPrenderParamVec4("uColor", &color);
	OPrenderParamMat4v("uWorld", 1, world);
}

void OPrenderTextColor4Mat4TextNode(OPfontUserTextNode* node, OPvec4 color, OPmat4* world) {
	OPrenderTextSetParameters(color, world);
	OPrenderBindMesh(&node->mesh);
	OPrenderBindEffect(OPRENDER_CURR_FONT_EFFECT);
	if (OPRENDER_CURR_FONT_MANAGER->pixelated) OPtexturePixelate();
	OPrenderMesh();
}

void OPrenderTextColor4Mat4BuiltNode(OPfontBuiltTextNode* node, OPvec4 color, OPmat4* world) {
	OPrenderTextSetParameters(color, world);
	OPmeshPackerBind(&OPRENDER_CURR_FONT_MANAGER->meshPacker);
	OPrenderBindEffect(OPRENDER_CURR_FONT_EFFECT);
	if (OPRENDER_CURR_FONT_MANAGER->pixelated) OPtexturePixelate();
	OPrenderMeshPacked(node->packedMesh);
}

void OPrenderTextAlign(OPmat4* world, OPfloat width, OPfontAlign align){
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

void OPrenderText(const OPchar* text, OPvec4 color, OPvec2 pos, OPfontAlign align) {
	ASSERT(OPRENDER_CURR_FONT_EFFECT != NULL, "A Font Effect has not been bound yet");
	ASSERT(OPRENDER_CURR_FONT_MANAGER != NULL, "A Font Manager has not been bound yet");
	ASSERT(OPRENDER_CURR_FONT_MANAGER->builtNodes != NULL, "The bound Font Manager Hashmap hasn't been created yet");

	int tryHashMap = OPRENDER_CURR_FONT_MANAGER->isBuilt;
	OPfontBuiltTextNode* node = NULL;
	if (tryHashMap) {
		OPhashMapGet(OPRENDER_CURR_FONT_MANAGER->builtNodes, text, (void**)&node);
	}

	OPmat4 world;
	OPfloat scale = (OPRENDER_WIDTH / 2.0f) * OPRENDER_SCREEN_WIDTH_SCALE;

	//OPmat4Scl(&world, OPrenderGetWidthAspectRatio() / scale, OPrenderGetHeightAspectRatio() / scale, 1.0f / scale);
	//OPmat4Translate(&world, pos.x, pos.y, 0.0f);

	if (node == NULL || !OPRENDER_CURR_FONT_MANAGER->isBuilt) {
		OPfontUserTextNode textNode = OPfontCreateUserText(OPRENDER_CURR_FONT_MANAGER->_font, text);
		OPrenderTextAlign(&world, textNode.Width, align);
		OPmat4Scl(&world, OPrenderGetWidthAspectRatio() / scale, OPrenderGetHeightAspectRatio() / scale, 1.0f);
		OPmat4Translate(&world, pos.x, pos.y, 0.0f);
		OPrenderTextColor4Mat4TextNode(&textNode, color, &world);
		OPrenderDestroyMesh(&textNode.mesh);
	}
	else {
		OPrenderTextAlign(&world, node->Width, align);
		OPmat4Scl(&world, OPRENDER_SCREEN_WIDTH_SCALE * OPrenderGetWidthAspectRatio() / scale, OPRENDER_SCREEN_HEIGHT_SCALE * OPrenderGetHeightAspectRatio() / scale, 1.0f);
		OPmat4Translate(&world, pos.x, pos.y, 0.0f);
		OPrenderTextColor4Mat4BuiltNode(node, color, &world);
	}
}

void OPrenderText(const OPchar* text, OPvec4 color, OPmat4* world) {
	ASSERT(OPRENDER_CURR_FONT_EFFECT != NULL, "A Font Effect has not been bound yet");
	ASSERT(OPRENDER_CURR_FONT_MANAGER != NULL, "A Font Manager has not been bound yet");
	ASSERT(OPRENDER_CURR_FONT_MANAGER->builtNodes != NULL, "The bound Font Manager Hashmap hasn't been created yet");

	int tryHashMap = OPRENDER_CURR_FONT_MANAGER->isBuilt;
	OPfontBuiltTextNode* node = NULL;
	if (tryHashMap) {
		OPhashMapGet(OPRENDER_CURR_FONT_MANAGER->builtNodes, text, (void**)&node);
	}

	OPmat4 aligned;
	OPmat4 scaled;
	OPfloat scale = (OPRENDER_WIDTH / 2.0f) * OPRENDER_SCREEN_WIDTH_SCALE;
	//OPlog("Scale %f, renderWidth %d, Width Scale %f, aspWidth %f, aspHeight %f", scale, OPrenderWidth, OPscreenWidthScale, OPrenderGetWidthAspectRatio(), OPrenderGetHeightAspectRatio());

	if (node == NULL || !OPRENDER_CURR_FONT_MANAGER->isBuilt) {
		OPfontUserTextNode textNode = OPfontCreateUserText(OPRENDER_CURR_FONT_MANAGER->_font, text);

		OPrenderTextAlign(&aligned, textNode.Width, OPRENDER_CURR_FONT_MANAGER->_align);
		//OPmat4Log("Aligned", &aligned);

		scaled = OPmat4Scl(
			OPrenderGetWidthAspectRatio() / scale, 
			OPrenderGetHeightAspectRatio() / scale, 
			1.0f);
		//OPmat4Log("Scaled", &scaled);

		OPmat4 temp;
		OPmat4Mul(&temp, scaled, aligned);
		OPmat4Mul(&temp, *world, temp);
		//OPmat4Log("World", world);
		//OPmat4Log("Temp", &temp);

		OPrenderTextColor4Mat4TextNode(&textNode, color, &temp);
		OPrenderDestroyMesh(&textNode.mesh);
	}
	else {
		OPrenderTextAlign(&aligned, node->Width, OPRENDER_CURR_FONT_MANAGER->_align);

		scaled = OPmat4Scl(
			OPrenderGetWidthAspectRatio() / scale,
			OPrenderGetHeightAspectRatio() / scale,
			1.0f);
		OPmat4 temp = scaled * aligned;
		temp = (*world) * temp;
		OPrenderTextColor4Mat4BuiltNode(node, color, &temp);
	}
}
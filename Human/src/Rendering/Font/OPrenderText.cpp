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
			OPmat4buildTranslate(world, 0, 0, 0.0f);
			break;
		case OPFONT_ALIGN_CENTER:
			OPmat4buildTranslate(world, -(width / 2.0f), 0, 0.0f);
			break;
		case OPFONT_ALIGN_RIGHT:
			OPmat4buildTranslate(world, -width, 0, 0.0f);
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
	OPfloat scale = (OPrenderWidth / 2.0f) * OPscreenWidthScale;

	//OPmat4scl(&world, OPrenderGetWidthAspectRatio() / scale, OPrenderGetHeightAspectRatio() / scale, 1.0f / scale);
	//OPmat4translate(&world, pos.x, pos.y, 0.0f);

	if (node == NULL || !OPRENDER_CURR_FONT_MANAGER->isBuilt) {
		OPfontUserTextNode textNode = OPfontCreateUserText(OPRENDER_CURR_FONT_MANAGER->_font, text);
		OPrenderTextAlign(&world, textNode.Width, align);
		OPmat4scl(&world, OPrenderGetWidthAspectRatio() / scale, OPrenderGetHeightAspectRatio() / scale, 1.0f);
		OPmat4translate(&world, pos.x, pos.y, 0.0f);
		OPrenderTextColor4Mat4TextNode(&textNode, color, &world);
		OPrenderDestroyMesh(&textNode.mesh);
	}
	else {
		OPrenderTextAlign(&world, node->Width, align);
		OPmat4scl(&world, OPscreenWidthScale * OPrenderGetWidthAspectRatio() / scale, OPscreenHeightScale * OPrenderGetHeightAspectRatio() / scale, 1.0f);
		OPmat4translate(&world, pos.x, pos.y, 0.0f);
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
	OPfloat scale = (OPrenderWidth / 2.0f) * OPscreenWidthScale;
	//OPlog("Scale %f, renderWidth %d, Width Scale %f, aspWidth %f, aspHeight %f", scale, OPrenderWidth, OPscreenWidthScale, OPrenderGetWidthAspectRatio(), OPrenderGetHeightAspectRatio());

	if (node == NULL || !OPRENDER_CURR_FONT_MANAGER->isBuilt) {
		OPfontUserTextNode textNode = OPfontCreateUserText(OPRENDER_CURR_FONT_MANAGER->_font, text);

		OPrenderTextAlign(&aligned, textNode.Width, OPRENDER_CURR_FONT_MANAGER->_align);
		//OPmat4Log("Aligned", &aligned);

		OPmat4buildScl(&scaled, 
			OPrenderGetWidthAspectRatio() / scale, 
			OPrenderGetHeightAspectRatio() / scale, 
			1.0f);
		//OPmat4Log("Scaled", &scaled);

		OPmat4 temp;
		OPmat4mul(&temp, &scaled, &aligned);
		OPmat4mul(&temp, world, &temp);
		//OPmat4Log("World", world);
		//OPmat4Log("Temp", &temp);

		OPrenderTextColor4Mat4TextNode(&textNode, color, &temp);
		OPrenderDestroyMesh(&textNode.mesh);
	}
	else {
		OPrenderTextAlign(&scaled, node->Width, OPRENDER_CURR_FONT_MANAGER->_align);
		OPmat4scl(&scaled, OPrenderGetWidthAspectRatio() / scale, OPrenderGetHeightAspectRatio() / scale, 1.0f);
		OPmat4 temp;
		OPmat4mul(&temp, &scaled, world);
		OPrenderTextColor4Mat4BuiltNode(node, color, &temp);
	}
}
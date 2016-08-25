#include "./Human/include/Rendering/Font/OPfontRender.h"
#include "./Human/include/Rendering/OPrender.h"

void OPfontRenderBegin(OPfontManager* fontManager) {
	ASSERT(fontManager != NULL, "A font manager must be used to begin");
	ASSERT(OPFONTMANAGER_EFFECT_ACTIVE != NULL, "A font effect must be used to begin");
	ASSERT(OPFONTMANAGER_ACTIVE == NULL, "A font manager has already been activated. Did you call OPfontRenderEnd?");
	
	OPFONTMANAGER_ACTIVE = fontManager;

	OPrenderDepth(0);
	fontManager->dummyMesh.mesh.Bind();
	OPFONTMANAGER_EFFECT_ACTIVE->Bind();
	//OPtextureClearActive();
	OPRENDERER_ACTIVE->Texture.Bind(OPFONTMANAGER_ACTIVE->_font->texture, 0);
	OPFONTMANAGER_EFFECT_ACTIVE->Set("uColorTexture", OPFONTMANAGER_ACTIVE->_font->texture, 0);
	OPFONTMANAGER_EFFECT_ACTIVE->Set("uColor", &OPFONTMANAGER_ACTIVE->_color);
	OPFONTMANAGER_EFFECT_ACTIVE->Set("uProj", &OPFONTMANAGER_ACTIVE->proj);
}

void OPfontRenderEnd() {
	OPFONTMANAGER_ACTIVE = NULL;
	OPrenderDepth(1);
}

void OPfontRender(OPfontUserTextNode* node, OPmat4* world) {
	OPFONTMANAGER_EFFECT_ACTIVE->Bind();
	OPFONTMANAGER_EFFECT_ACTIVE->Set("uWorld", world);
	node->mesh.Bind();
	OPrenderDrawBufferIndexed(0);
}

void OPfontRender(OPfontBuiltTextNode* node, OPmat4* world) {
	OPFONTMANAGER_EFFECT_ACTIVE->Bind();
	OPFONTMANAGER_EFFECT_ACTIVE->Set("uWorld", world); 
	OPFONTMANAGER_ACTIVE->meshPacker.Bind();
	OPRENDERER_ACTIVE->VertexArray.SetLayout(&OPFONTMANAGER_ACTIVE->meshPacker.vertexArray, &node->packedMesh->vertexLayout);
	node->packedMesh->Render();
}

void OPfontRenderSetAlign(OPmat4* world, OPfloat width, OPfontAlign align){
	switch (align) {
	case OPfontAlign::LEFT:
			*world = OPmat4Translate(0, 0, 0.0f);
			break;
	case OPfontAlign::CENTER:
			*world = OPmat4Translate(-(width / 2.0f), 0, 0.0f);
			break;
	case OPfontAlign::RIGHT:
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
		OPFONTMANAGER_ACTIVE->builtNodes->Get(text, (void**)&node);
	}

	OPmat4 aligned;

	if (node == NULL || !OPFONTMANAGER_ACTIVE->isBuilt) {
		OPfontUserTextNode textNode = OPFONTMANAGER_ACTIVE->_font->CreateUserText(text, 1.0);
		OPfontRenderSetAlign(&aligned, textNode.Width, OPFONTMANAGER_ACTIVE->_align);
		OPmat4 temp = (*world) * OPmat4Scl(OPFONTMANAGER_ACTIVE->scale) * aligned;
		OPfontRender(&textNode, &temp);
	}
	else {
		OPfontRenderSetAlign(&aligned, node->Width, OPFONTMANAGER_ACTIVE->_align);
		OPmat4 temp = (*world) * OPmat4Scl(OPFONTMANAGER_ACTIVE->scale) * aligned;
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
		OPFONTMANAGER_ACTIVE->builtNodes->Get(text, (void**)&node);
	}

	OPmat4 aligned;

	if (node == NULL || !OPFONTMANAGER_ACTIVE->isBuilt) {
		OPfontUserTextNode textNode = OPFONTMANAGER_ACTIVE->_font->CreateUserText(text, 1.0);
		OPfontRender(&textNode, world);
	}
	else {
		OPfontRender(node, world);
	}
}
#include "./Human/include/Systems/OPfontSystem.h"
#include "./Data/include/OPcman.h"
#include "./Core/include/Assert.h"

#include "./Human/include/Rendering/Font/OPfontManager.h"
#include "./Human/include/Rendering/OPeffect.h"

const OPchar* FontShaderVert = "Common/TexturedScreen.vert";
// const OPchar* FontShaderScreenVert = "Common/TexturedScreen.vert";
const OPchar* FontShaderFrag = "Common/Font.frag";

void OPfontSystemLoadEffects() {

	if (OPFONTMANAGER_EFFECT_ACTIVE != NULL) {
		return; 
	}

	OPint error;

	if (!OPCMAN.IsLoaded(FontShaderVert)) {
		error = OPCMAN.Load(FontShaderVert);
		ASSERT(error, "Failed to load Vertex Shader for Font Effect");
	}

	if (!OPCMAN.IsLoaded(FontShaderFrag)) {
		error = OPCMAN.Load(FontShaderFrag);
		ASSERT(error, "Failed to load Fragment Shader for Font Effect");
	}

	//OPshaderAttribute attribs[] = {
	//	{ "aPosition", OPshaderElementType::FLOAT, 3 },
	//	{ "aUV", OPshaderElementType::FLOAT, 2 }
	//};

	OPFONTMANAGER_EFFECT_ACTIVE = (OPeffect*)OPalloc(sizeof(OPeffect));
	OPFONTMANAGER_EFFECT_ACTIVE->Init((OPshader*)OPCMAN.Get(FontShaderVert), (OPshader*)OPCMAN.Get(FontShaderFrag));

	//OPFONTMANAGER_EFFECT_ACTIVE->AddUniform("uColorTexture");
	//OPFONTMANAGER_EFFECT_ACTIVE->AddUniform("uColor");
	//OPFONTMANAGER_EFFECT_ACTIVE->AddUniform("uWorld");
	//OPFONTMANAGER_EFFECT_ACTIVE->AddUniform("uView");
	//OPFONTMANAGER_EFFECT_ACTIVE->AddUniform("uProj");
}

void OPfontSystemShutdownEffects() {
	OPFONTMANAGER_EFFECT_ACTIVE->Destroy();
	OPfree(OPFONTMANAGER_EFFECT_ACTIVE);
	OPFONTMANAGER_EFFECT_ACTIVE = NULL;
}
// void OPfontSystemLoadScreenEffects() {

// 	if (OPFONTMANAGER_EFFECT_ACTIVE != NULL) { 
// 		OPeffectBind(OPFONTMANAGER_EFFECT_ACTIVE);
// 		return; 
// 	}

// 	OPint error;

// 	if (!OPCMAN.IsLoaded(FontShaderScreenVert)) {
// 		error = OPCMAN.Load(FontShaderScreenVert);
// 		ASSERT(error, "Failed to load Vertex Shader for Font Effect");
// 	}

// 	if (!OPCMAN.IsLoaded(FontShaderFrag)) {
// 		error = OPCMAN.Load(FontShaderFrag);
// 		ASSERT(error, "Failed to load Fragment Shader for Font Effect");
// 	}

// 	OPshaderAttribute attribs[] = {
// 		{ "aPosition", GL_FLOAT, 3 },
// 		{ "aUV", GL_FLOAT, 2 }
// 	};

// 	OPFONTMANAGER_EFFECT_ACTIVE = (OPeffect*)OPalloc(sizeof(OPeffect));
// 	*OPFONTMANAGER_EFFECT_ACTIVE = OPeffectCreate(
// 		*(OPshader*)OPCMAN.Get(FontShaderScreenVert),
// 		*(OPshader*)OPCMAN.Get(FontShaderFrag),
// 		attribs,
// 		2,
// 		"Common Font Effect"
// 		);

// }

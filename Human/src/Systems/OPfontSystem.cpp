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

	if (!OPcmanIsLoaded(FontShaderVert)) {
		error = OPcmanLoad(FontShaderVert);
		ASSERT(error, "Failed to load Vertex Shader for Font Effect");
	}

	if (!OPcmanIsLoaded(FontShaderFrag)) {
		error = OPcmanLoad(FontShaderFrag);
		ASSERT(error, "Failed to load Fragment Shader for Font Effect");
	}

	OPshaderAttribute attribs[] = {
		{ "aPosition", GL_FLOAT, 3 },
		{ "aUV", GL_FLOAT, 2 }
	};

	OPFONTMANAGER_EFFECT_ACTIVE = (OPeffect*)OPalloc(sizeof(OPeffect));
	*OPFONTMANAGER_EFFECT_ACTIVE = OPeffectCreate(
		*(OPshader*)OPcmanGet(FontShaderVert),
		*(OPshader*)OPcmanGet(FontShaderFrag),
		attribs,
		2,
		"Common Font Effect"
		);

}

// void OPfontSystemLoadScreenEffects() {

// 	if (OPFONTMANAGER_EFFECT_ACTIVE != NULL) { 
// 		OPeffectBind(OPFONTMANAGER_EFFECT_ACTIVE);
// 		return; 
// 	}

// 	OPint error;

// 	if (!OPcmanIsLoaded(FontShaderScreenVert)) {
// 		error = OPcmanLoad(FontShaderScreenVert);
// 		ASSERT(error, "Failed to load Vertex Shader for Font Effect");
// 	}

// 	if (!OPcmanIsLoaded(FontShaderFrag)) {
// 		error = OPcmanLoad(FontShaderFrag);
// 		ASSERT(error, "Failed to load Fragment Shader for Font Effect");
// 	}

// 	OPshaderAttribute attribs[] = {
// 		{ "aPosition", GL_FLOAT, 3 },
// 		{ "aUV", GL_FLOAT, 2 }
// 	};

// 	OPFONTMANAGER_EFFECT_ACTIVE = (OPeffect*)OPalloc(sizeof(OPeffect));
// 	*OPFONTMANAGER_EFFECT_ACTIVE = OPeffectCreate(
// 		*(OPshader*)OPcmanGet(FontShaderScreenVert),
// 		*(OPshader*)OPcmanGet(FontShaderFrag),
// 		attribs,
// 		2,
// 		"Common Font Effect"
// 		);

// }

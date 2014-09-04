#include "./Human/include/Systems/FontSystem.h"
#include "./Data/include/ContentManager.h"
#include "./Core/include/Assert.h"

#include "./Human/include/Rendering/Font/FontManager.h"
#include "./Human/include/Rendering/Effect.h"

const OPchar* FontShaderVert = "Common/TexturedScreen.vert";
const OPchar* FontShaderFrag = "Common/Font.frag";

void OPsystemsLoadFontEffect() {

	if (OPRENDER_CURR_FONT_EFFECT != NULL) { 
		OPrenderBindEffect(OPRENDER_CURR_FONT_EFFECT);
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

	OPRENDER_CURR_FONT_EFFECT = (OPeffect*)OPalloc(sizeof(OPeffect));
	*OPRENDER_CURR_FONT_EFFECT = OPrenderCreateEffect(
		*(OPshader*)OPcmanGet(FontShaderVert),
		*(OPshader*)OPcmanGet(FontShaderFrag),
		attribs,
		2,
		"Common Font Effect"
		);

}

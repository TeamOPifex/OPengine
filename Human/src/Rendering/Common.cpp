#include "./Human/include/Rendering/Common.h"
#include "./Data/include/ContentManager.h"
#include "./Core/include/Assert.h"

#include "./Human/include/Rendering/Font/FontManager.h"
#include "./Human/include/Rendering/Effect.h"

const i8* FontShaderVert = "Common/TexturedScreen.vert";
const i8* FontShaderFrag = "Common/Font.frag";

void OPcommonLoadFontEffect() {

	ASSERT(OPRENDER_CURR_FONT_EFFECT == NULL, "A Font has already been set");

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
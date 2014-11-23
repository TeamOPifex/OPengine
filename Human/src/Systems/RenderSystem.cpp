#include "./Human/include/Systems/RenderSystem.h"
#include "./Data/include/ContentManager.h"

OPeffect OPeffectLoadTextured2D(ui32 vertexStride) {
	OPshaderAttribute attribs[] = {
		{ "aPosition", GL_FLOAT, 3 },
		{ "aNormal", GL_FLOAT, 3 },
		{ "aUV", GL_FLOAT, 2 }
	};

	OPcmanLoad("Common/Texture2D.vert");
	OPcmanLoad("Common/Texture.frag");
	OPshader* vert = (OPshader*)OPcmanGet("Common/Texture2D.vert");
	OPshader* frag = (OPshader*)OPcmanGet("Common/Texture.frag");
	OPeffect effect = OPrenderCreateEffectStride(
		*vert,
		*frag,
		attribs,
		3,
		"Textured Effect",
		vertexStride
		);

	return effect;
}

OPeffect OPeffectLoadTextured3D(ui32 vertexStride) {
	OPshaderAttribute attribs[] = {
		{ "aPosition", GL_FLOAT, 3 },
		{ "aNormal", GL_FLOAT, 3 },
		{ "aUV", GL_FLOAT, 2 }
	};

	OPcmanLoad("Common/Texture3D.vert");
	OPcmanLoad("Common/Texture.frag");
	OPshader* vert = (OPshader*)OPcmanGet("Common/Texture3D.vert");
	OPshader* frag = (OPshader*)OPcmanGet("Common/Texture.frag");
	OPeffect effect = OPrenderCreateEffectStride(
		*vert,
		*frag,
		attribs,
		3,
		"Textured Effect",
		vertexStride
		);

	return effect;
}
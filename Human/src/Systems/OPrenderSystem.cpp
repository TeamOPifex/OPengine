#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Data/include/OPcman.h"

OPeffect OPrenderSystemLoadTextured2D(ui32 vertexStride) {
	OPshaderAttribute attribs[] = {
		{ "aPosition", OPshaderElementType::FLOAT, 3 },
		{ "aNormal", OPshaderElementType::FLOAT, 3 },
		{ "aUV", OPshaderElementType::FLOAT, 2 }
	};

	OPCMAN.Load("Common/Texture2D.vert");
	OPCMAN.Load("Common/Texture.frag");
	OPshader* vert = (OPshader*)OPCMAN.Get("Common/Texture2D.vert");
	OPshader* frag = (OPshader*)OPCMAN.Get("Common/Texture.frag");

	OPeffect effect;
	effect.Init(vert, frag);

	return effect;
}

OPeffect OPrenderSystemLoadTextured3D(ui32 vertexStride) {
	OPshaderAttribute attribs[] = {
		{ "aPosition", OPshaderElementType::FLOAT, 3 },
		{ "aNormal", OPshaderElementType::FLOAT, 3 },
		{ "aUV", OPshaderElementType::FLOAT, 2 }
	};

	OPCMAN.Load("Common/Texture3D.vert");
	OPCMAN.Load("Common/Texture.frag");
	OPshader* vert = (OPshader*)OPCMAN.Get("Common/Texture3D.vert");
	OPshader* frag = (OPshader*)OPCMAN.Get("Common/Texture.frag");

	OPeffect effect;
	effect.Init(vert, frag);

	return effect;
}
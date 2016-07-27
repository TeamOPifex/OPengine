#ifndef OPENGINE_HUMAN_RENDERING_EFFECT_API
#define OPENGINE_HUMAN_RENDERING_EFFECT_API

#include "./Core/include/OPtypes.h"

struct OPeffectAPI;
typedef struct OPeffectAPI OPeffectAPI;

struct OPeffect;
#include "./Human/include/Rendering/API/OPshaderAPI.h"

struct OPeffectAPI {
	OPeffect*(*Create)(OPshader* vert, OPshader* frag) = 0;
	OPeffect*(*Init)(OPeffect* effect, OPshader* vert, OPshader* frag) = 0;
	void(*Bind)(OPeffect* effect) = 0;
	void(*Unbind)(OPeffect* effect) = 0;
	void(*Destroy)(OPeffect* effect) = 0;
};

#endif
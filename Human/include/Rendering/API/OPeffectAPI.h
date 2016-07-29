#ifndef OPENGINE_HUMAN_RENDERING_EFFECT_API
#define OPENGINE_HUMAN_RENDERING_EFFECT_API

#include "./Core/include/OPtypes.h"

struct OPeffectAPI;
typedef struct OPeffectAPI OPeffectAPI;

struct OPeffect;
struct OPshader;
struct OPeffectAttribute;

struct OPeffectAPI {
	OPeffect*(*Create)(OPshader* vert, OPshader* frag) = 0;
	OPeffect*(*Init)(OPeffect* effect, OPshader* vert, OPshader* frag) = 0;
	bool(*AddUniform)(OPeffect* effect, const OPchar* name) = 0;
	void(*Bind)(OPeffect* effect) = 0;
	void(*Unbind)(OPeffect* effect) = 0;
	void(*Destroy)(OPeffect* effect) = 0;
};

#endif
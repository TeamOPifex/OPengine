#pragma once

struct OPeffectAPI;
typedef struct OPeffectAPI OPeffectAPI;

#include "./Core/include/OPtypes.h"

struct OPeffect;
struct OPshader;
struct OPeffectAttribute;
struct OPvertexLayout;

struct OPeffectAPI {
	OPeffect*(*Create)(OPshader* vert, OPshader* frag) = 0;
	OPeffect*(*Init)(OPeffect* effect, OPshader* vert, OPshader* frag) = 0;
	bool(*AddUniform)(OPeffect* effect, const OPchar* name) = 0;
	void(*Bind)(OPeffect* effect) = 0;
    void(*SetVertexLayout)(OPeffect* effect, OPvertexLayout* vertexLayout) = 0;
	void(*Unbind)(OPeffect* effect) = 0;
	void(*Destroy)(OPeffect* effect) = 0;
};
#pragma once

struct OPeffectAPI;
typedef struct OPeffectAPI OPeffectAPI;

#include "./Core/include/OPtypes.h"

struct OPeffect;
struct OPshader;
struct OPeffectAttribute;
struct OPvertexLayout;

struct OPeffectAPI {
	OPeffect*(*_Create)(OPshader* vert, OPshader* frag, OPvertexLayout* vertexLayout) = 0;
	OPeffect*(*_Init)(OPeffect* effect, OPshader* vert, OPshader* frag, OPvertexLayout* vertexLayout) = 0;
	bool(*AddUniform)(OPeffect* effect, const OPchar* name) = 0;
	void(*Bind)(OPeffect* effect) = 0;
    void(*SetVertexLayout)(OPeffect* effect, OPvertexLayout* vertexLayout) = 0;
	void(*Unbind)(OPeffect* effect) = 0;
	void(*Destroy)(OPeffect* effect) = 0;

	inline OPeffect* Create(OPshader* vert, OPshader* frag) {
		return _Create(vert, frag, NULL);
	}
	inline OPeffect* Create(OPshader* vert, OPshader* frag, OPvertexLayout* vertexLayout) {
		return _Create(vert, frag, vertexLayout);
	}
	inline OPeffect* Init(OPeffect* effect, OPshader* vert, OPshader* frag) {
		return _Init(effect, vert, frag, NULL);
	}
	inline OPeffect* Init(OPeffect* effect, OPshader* vert, OPshader* frag, OPvertexLayout* vertexLayout) {
		return _Init(effect, vert, frag, vertexLayout);
	}
};
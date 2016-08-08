#ifndef OPENGINE_HUMAN_RENDERING_TEXTURE_CUBE_API
#define OPENGINE_HUMAN_RENDERING_TEXTURE_CUBE_API

#include "./Core/include/OPtypes.h"

struct OPtextureCubeAPI;
typedef struct OPtextureCubeAPI OPtextureCubeAPI;

struct OPtextureCube;
//struct OPimage;

struct OPtextureCubeAPI {
	//OPtextureCube*(*Create)(OPimage* images) = 0;
	//void(*Init)(OPtextureCube* textureCube) = 0;
	//void(*Bind)(OPtextureCube* textureCube) = 0;
	void(*Unbind)(OPtextureCube* textureCube) = 0;
	void(*Unbind1)(OPtextureCube* textureCube) = 0;
	void(*Unbind2)(OPtextureCube* textureCube) = 0;
	void(*Unbind3)(OPtextureCube* textureCube) = 0;
	void(*Unbind4)(OPtextureCube* textureCube) = 0;
	void(*Unbind5)(OPtextureCube* textureCube) = 0;
	void(*Unbind6)(OPtextureCube* textureCube) = 0;
	void(*Unbind7)(OPtextureCube* textureCube) = 0;
	void(*Unbind8)(OPtextureCube* textureCube) = 0;
	void(*Unbind9)(OPtextureCube* textureCube) = 0;
	void(*Unbind10)(OPtextureCube* textureCube) = 0;
	void(*Unbind11)(OPtextureCube* textureCube) = 0;
	void(*Unbind12)(OPtextureCube* textureCube) = 0;
	void(*Unbind13)(OPtextureCube* textureCube) = 0;
	void(*Unbind14)(OPtextureCube* textureCube) = 0;
	void(*Unbind15)(OPtextureCube* textureCube) = 0;
	void(*Unbind16)(OPtextureCube* textureCube) = 0;
	void(*Unbind17)(OPtextureCube* textureCube) = 0;
	void(*Unbind18)(OPtextureCube* textureCube) = 0;
	void(*Unbind19)(OPtextureCube* textureCube) = 0;
};

#endif
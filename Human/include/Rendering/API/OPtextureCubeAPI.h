#ifndef OPENGINE_HUMAN_RENDERING_TEXTURE_CUBE_API
#define OPENGINE_HUMAN_RENDERING_TEXTURE_CUBE_API

#include "./Core/include/OPtypes.h"

struct OPtextureCubeAPI;
typedef struct OPtextureCubeAPI OPtextureCubeAPI;

struct OPtextureCube;
struct OPimage;

struct OPtextureCubeAPI {
	OPtextureCube*(*_Create)(OPimage* textures) = 0;
	OPtextureCube*(*_Init)(OPtextureCube* texture, OPimage* textures) = 0;
	void(*Bind)(OPtextureCube* ptr, ui32 slot) = 0;
	void(*Unbind)(OPtextureCube* ptr, ui32 slot) = 0;
	void(*Destroy)(OPtextureCube* ptr) = 0;
	
	inline OPtextureCube* Create(OPimage* textures) { return _Create(textures); }
	OPtextureCube* Create(const OPchar** textures);
		
	inline OPtextureCube* Init(OPtextureCube* textureCube, OPimage* textures) { return _Init(textureCube, textures); }
	OPtextureCube* Init(OPtextureCube* textureCube, const OPchar** textures);

};

#endif
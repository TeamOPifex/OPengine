#ifndef OPENGINE_HUMAN_RENDERING_TEXTURE
#define OPENGINE_HUMAN_RENDERING_TEXTURE

#include "./Core/include/OPtypes.h"

struct OPtexture;
typedef struct OPtexture OPtexture;

enum struct OPtextureWrap
{
	NONE = 0,
	REPEAT,
	CLAMP,
	MIRRORED_REPEAT,
	CLAMP_TO_EDGE,
	CLAMP_TO_BORDER
};

enum struct OPtextureFilter
{
	NONE = 0,
	LINEAR,
	NEAREST
};

enum struct OPtextureFormat
{
	NONE = 0,
	RGB,
	RGBA,
	LUMINANCE,
	LUMINANCE_ALPHA
};

struct OPtexture {
	void(*Bind)(void* ptr, ui32 slot);
	void(*Unbind)(void* ptr, ui32 slot);
};

#endif
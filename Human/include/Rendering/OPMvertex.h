#ifndef OPENGINE_HUMAN_RENDERER_OPM_VERTEX
#define OPENGINE_HUMAN_RENDERER_OPM_VERTEX

#include "./Math/include/Vectors.h"

typedef struct {
	OPvec3 Position;
	OPvec3 Normal;
	OPvec3 Tangent;
	OPvec2 TexCoord;
} OPMvertex;


typedef struct {
	OPvec3 Position;
	OPvec3 Normal;
	OPvec3 Tangent;
	OPvec2 TexCoord;
	OPvec4 Bones;
	OPvec4 BoneWeights;
} OPMvertexSkin;

typedef struct {
	OPvec3 Position;
	OPvec2 TexCoord;
} OPvertexColor;

#endif
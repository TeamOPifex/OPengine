#ifndef OPENGINE_HUMAN_RENDERER_OPM_VERTEX
#define OPENGINE_HUMAN_RENDERER_OPM_VERTEX

#include "./Math/include/Vectors.h"

enum OPMFeatures {
	Position = 0x01,
	Normal = 0x02,
	UV = 0x04,
	Tangent = 0x08,
	Index = 0x10,
	Bones = 0x20,
	Skinning = 0x40,
	Animations = 0x80,
	Color = 0x100,
	Meta = 0x200
};

enum OPMKeyFeatures {
	Key_Time = 0x01,
	Key_Position = 0x02,
	Key_Rotation = 0x04
};

OPint OPMhasFeature(ui32 features, ui32 feature);

struct OPMmeta {
	OPchar* Name;
	OPchar* Type;
	OPvec3 Position;
	OPvec3 Rotation;
	OPvec3 Scale;
};

struct OPMvertex {
	OPvec3 Position;
	OPvec3 Normal;
	OPvec3 Tangent;
	OPvec2 TexCoord;
};

struct OPMvertexNormalUV {
	OPvec3 Position;
	OPvec3 Normal;
	OPvec2 UV;
};

struct OPMvertexColor {
	OPvec3 Position;
	OPvec3 Normal;
	OPvec3 Tangent;
	OPvec3 Color;
};


struct OPMvertexSkin {
	OPvec3 Position;
	OPvec3 Normal;
	OPvec3 Tangent;
	OPvec2 TexCoord;
	OPvec4 Bones;
	OPvec4 BoneWeights;
};

struct OPvertexTex {
	OPvec3 Position;
	OPvec2 TexCoord;
};

struct OPvertices {
	i8* data;
	ui32 features;
	ui32 size;
	ui32 count;
};

OPvertices* OPverticesCreate(ui32 count, ui32 features);
void OPverticesWriteVec2(OPvertices* vertices, OPvec2* data, ui32 feature);
void OPverticesWriteVec3(OPvertices* vertices, OPvec3* data, ui32 feature);
void OPverticesWriteVec4(OPvertices* vertices, OPvec4* data, ui32 feature);
#endif

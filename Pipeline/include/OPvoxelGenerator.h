#ifndef OPVOXEL_GENERATOR
#define OPVOXEL_GENERATOR

#include "./OPengine.h"
#include "./Pipeline/include/Loaders/OPloaderVoxels.h"

#define Z_SLICE voxelData.size.x * voxelData.size.y * z
#define Y_LINE  voxelData.size.x * y

#define VAL(X, Y, Z) (voxelData.voxels[(X) + voxelData.size.x * (Y) + voxelData.size.x * voxelData.size.y * (Z)])
#define IS_IN_BOUNDS(X, Y, Z) (X) >= 0 && (X) < voxelData.size.x && (Y) >= 0 && (Y) < voxelData.size.y && (Z) >= 0 && (Z) < voxelData.size.z
#define IS_BLOCK_INVISIBLE(block) (block.x == 0 && block.y == 0 && block.z == 0)

enum VoxelFaces {
	vLeft = 0,
	vRight,
	vUp,
	vDown,
	vForward,
	vBackward
};

struct OPvoxelGenerator {
	OPuint Features;
	OPuint VertexSize;
	OPuint IndexOffset;
	void* Vertex;
	OPlist* Vertices, *Indices;
};

struct OPvoxelGeneratorVertex {
	OPvec3 Position;
	OPvec3 Normal;
	OPvec3 Tangent;
	OPvec2 UV;
	OPvec3 Color;
	OPvec4 Bones;
	OPvec4 Weights;
};

struct OPvoxelGeneratorData {
	struct OPvoxelGeneratorVertex Vertices[4];
	ui32 Indices[6];
	OPvec3 Offset;
	OPvec3 Color;
	OPfloat Size;
};

void OPvoxelGeneratorInit(struct OPvoxelGenerator* gen, OPuint features);
struct OPvoxelGenerator* OPvoxelGeneratorCreate(OPuint features);
void OPvoxelGeneratorAdd(struct OPvoxelGenerator* gen, struct OPvoxels voxelData, i8 center, OPvec4 bones, OPvec4 weights, OPvec3 offset, OPfloat scale);
OPmesh* OPvoxelGeneratorBuild(struct OPvoxelGenerator* gen);
void OPvoxelGeneratorDestroy(struct OPvoxelGenerator* gen);

inline void OPvoxelGeneratorAdd(struct OPvoxelGenerator* gen, struct OPvoxels voxelData, i8 center, OPvec4 bones, OPvec4 weights) {
	OPvoxelGeneratorAdd(gen, voxelData, center, bones, weights, OPVEC3_ZERO, 1.0);
}
inline void OPvoxelGeneratorAdd(struct OPvoxelGenerator* gen, struct OPvoxels voxelData, i8 center) {
	OPvoxelGeneratorAdd(gen, voxelData, center, OPvec4Zero, OPvec4Zero);
}
inline void OPvoxelGeneratorAdd(struct OPvoxelGenerator* gen, struct OPvoxels voxelData, i8 center, OPvec3 offset) {
	OPvoxelGeneratorAdd(gen, voxelData, center, OPvec4Zero, OPvec4Zero, offset, 1.0);
}

#endif

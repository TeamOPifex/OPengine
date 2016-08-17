#pragma once

struct OPsphericalCube;
struct OPsphericalCubeVertex;

typedef struct OPsphericalCube OPsphericalCube;
typedef struct OPsphericalCubeVertex OPsphericalCubeVertex;

enum struct OPsphericalCubeSide
{
	TOP = 0,
	BOTTOM = 1,
	LEFT = 2,
	RIGHT = 3,
	FRONT = 4,
	BACK = 5,
	_MAX
};

#define OPSPHERICALCUBE_SCALE 1.0f
#define OPSPHERICALCUBE_SCL 1

#include "./Human/include/Rendering/OPmesh.h"
#include "./Human/include/Rendering/OPimage.h"
#include "./Math/include/OPvec2.h"
#include "./Math/include/OPvec3.h"

struct OPsphericalCubeVertex {
	OPvec3 pos;
	OPvec3 norm;
	OPvec2 uv;
};

struct OPsphericalCube {
	OPmesh sides[(ui32)OPsphericalCubeSide::_MAX];

	OPsphericalCube() { }
	OPsphericalCube(OPimage* faces) { Init(faces); }
	OPsphericalCube(ui16 size) { Init(size); }

	void Init(OPimage* faces);
	void Init(ui16 size);
	void Destroy();

	static OPvec3 Position(OPvec3 pos, OPsphericalCubeSide* side);
	static OPvec2 PlanePositionSide(const OPvec3 pos, OPsphericalCubeSide side);
	static OPvec2 PlanePosition(const OPvec3 pos, OPsphericalCubeSide* side);

	inline static OPsphericalCube* Create(OPimage* faces) { return OPNEW(OPsphericalCube(faces)); }
	inline static OPsphericalCube* Create(ui16 size) { return OPNEW(OPsphericalCube(size)); }
};
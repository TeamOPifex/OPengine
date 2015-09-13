#include "./Pipeline/include/Loaders/OPloaderVoxels.h"
#include "./OPengine.h"

OPchar* readMatrixName(OPstream* str) {
	OPchar len = *OPread(str, sizeof(ui8));
	i8* name = (i8*)OPalloc(len + 1);
	OPmemcpy(name, OPread(str, len), len);
	name[len] = '\0';
	return name;
}

struct OPvecInt3 readVoxelVec3(OPstream* str) {
	OPvecInt3 temp = {
		(OPint)OPreadui32(str),
		(OPint)OPreadui32(str),
		(OPint)OPreadui32(str)
	};
	return temp;
}

OPvec3* readColorData(OPstream* str) {

	OPvec3* result = NULL;

	OPchar* name = readMatrixName(str);
	OPfree(name);

	OPvecInt3 voxelSize = readVoxelVec3(str);
	OPvecInt3 voxelOffset = readVoxelVec3(str);

	result = (OPvec3*)OPalloc(sizeof(OPvec3) * voxelSize.x * voxelSize.z * voxelSize.y);

	// read voxel data
	for(int zi = 0; zi < voxelSize.z; ++zi){
		for(int yi = 0; yi < voxelSize.y; ++yi){
			for(int xi = 0; xi < voxelSize.x; ++xi){
				ui32 data = OPreadui32(str);
				ui32 pos = xi + yi * voxelSize.x + (voxelSize.z - 1 - zi) * voxelSize.x * voxelSize.y;
				result[pos] = OPvec3Create(((ui8*)&data)[0], ((ui8*)&data)[1], ((ui8*)&data)[2]);
				//OPlog("rgb - (%d, %d, %d)", ((ui8*)&data)[0], ((ui8*)&data)[1], ((ui8*)&data)[2]);
			}
		}
	}

	return result;
}

OPvoxels readVoxelData(OPstream* str) {

	OPvoxels result;

	OPchar* name = readMatrixName(str);
	OPfree(name);

	OPvecInt3 voxelSize = readVoxelVec3(str);
	OPvecInt3 voxelOffset = readVoxelVec3(str);

	OPvecInt3* voxels = (OPvecInt3*)OPalloc(sizeof(OPvecInt3) * voxelSize.x * voxelSize.z * voxelSize.y);

	// read voxel data
	for(int zi = 0; zi < voxelSize.z; ++zi){
		for(int yi = 0; yi < voxelSize.y; ++yi){
			for(int xi = 0; xi < voxelSize.x; ++xi){
				ui32 data = OPreadui32(str);
				//if(data != 0) OPlog("rgb - (%d, %d, %d)", ((ui8*)&data)[0], ((ui8*)&data)[1], ((ui8*)&data)[2]);
				OPvecInt3 voxel = {
					((ui8*)&data)[0],
					((ui8*)&data)[1],
					((ui8*)&data)[2]
				};
				voxels[(voxelSize.x - 1 - xi) + yi * voxelSize.x + voxelSize.x * voxelSize.y * (voxelSize.z - 1 - zi)] = voxel;
			}
		}
	}

	result.size = voxelSize;
	result.offset = voxelOffset;
	result.voxels = voxels;

	return result;
}

OPint OPvoxelsLoad(const OPchar* path, void** asset) {

	OPstream* str = OPreadFile(path);

	ui32 version = OPreadui32(str);
	ui32 colorFormat = OPreadui32(str);
	ui32 zOrientation = OPreadui32(str);
	ui32 compressed = OPreadui32(str);
	ui32 visibilityMsk = OPreadui32(str);
	ui32 matrixCount = OPreadui32(str);

	// OPlog("\tVersion: %u\n\tColor Frmt: %u\n\tZ ori: %u\n\tCompressed: %u\n\tVis Msk: %u\n\tMat count: %u\n",
	// 	version,
	// 	colorFormat,
	// 	zOrientation,
	// 	compressed,
	// 	visibilityMsk,
	// 	matrixCount
	// );

	ui32 width;
	ui32 height;
	ui32 depth;

	OPvec3* colors = readColorData(str);
	OPfree(colors);

	OPvoxels* temp = (OPvoxels*)OPalloc(sizeof(OPvoxels));
	*temp = readVoxelData(str);
	*asset = temp;

	return 1;
}

OPvecInt3 OPvoxelsGet(OPvoxels* voxels, OPint x, OPint y, OPint z) {
	if(x < 0 || x > voxels->size.x || y < 0 || y > voxels->size.y || z < 0 || z > voxels->size.z) {
		OPvecInt3 z = { 0, 0, 0 };
		return z;
	}
	return voxels->voxels[x + y * voxels->size.x + voxels->size.x * voxels->size.y * z];
}

OPassetLoader* OPvoxelsLoader() {
	static OPassetLoader loader = {
		".qb",
		"Voxels/",
		sizeof(OPvoxels),
		OPvoxelsLoad,
		NULL,
		NULL
	};
	return &loader;
}

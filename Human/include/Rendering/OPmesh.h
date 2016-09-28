#pragma once

struct OPmesh;
typedef struct OPmesh OPmesh;

#include "./Human/include/Rendering/OPrenderBuffer.h"
#include "./Math/include/OPboundingBox3D.h"
#include "./Human/include/Rendering/Skinning/OPskeleton.h"
#include "./Human/include/Rendering/Skinning/OPskeletonAnimation.h"
#include "./Human/include/Rendering/OPMvertex.h"
#include "./Human/include/Rendering/OPvertexLayout.h"
#include "./Human/include/Rendering/OPvertexArray.h"
#include "./Human/include/Rendering/Enums/OPmeshMetaType.h"

extern OPmesh* OPMESH_ACTIVE; 

struct OPmeshDesc {
	void* vertices;
	void* indices;

	ui32 vertexCount;
	ui32 indexCount;

	OPindexSize indexSize;
};

struct OPmeshMeta {
	ui16 count;
	OPmeshMetaType* metaType;
	OPstream** data;
};

struct OPmeshMaterialDesc {
	OPchar* diffuse = NULL;
	OPchar* specular = NULL;
	OPchar* ambient = NULL;
	OPchar* emissive = NULL;
	OPchar* height = NULL;
	OPchar* normals = NULL;
	OPchar* shininess = NULL;
	OPchar* opacity = NULL;
	OPchar* displacement = NULL;
	OPchar* lightMap = NULL;
	OPchar* reflection = NULL;
	OPchar* other1 = NULL;
	OPchar* other2 = NULL;
	OPchar* other3 = NULL;
};

struct OPmesh {
	ui64 id;
	OPuint offset;
	OPuint count;
	OPboundingBox3D boundingBox;
	OPchar* name;

	OPvertexArray* vertexArray;
	OPvertexBuffer* vertexBuffer;
	OPindexBuffer* indexBuffer;

	OPmeshDesc* meshDesc;
	OPmeshMaterialDesc* materialDesc;
	OPmeshMeta* meshMeta;
	
	void Bind();
	void Draw();
};
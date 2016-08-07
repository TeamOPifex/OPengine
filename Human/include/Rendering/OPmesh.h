#ifndef OPENGINE_HUMAN_RENDERER_MESH
#define OPENGINE_HUMAN_RENDERER_MESH

#include "./Human/include/Rendering/OPrenderBuffer.h"
#include "./Math/include/OPboundingBox3D.h"
#include "./Human/include/Rendering/Skinning/OPskeleton.h"
#include "./Human/include/Rendering/Skinning/OPskeletonAnimation.h"
#include "./Human/include/Rendering/OPMvertex.h"
#include "./Human/include/Rendering/OPvertexLayout.h"

struct OPmeshDataMeta {
	OPchar* Name;
	OPchar* Type;
	OPvec3 Position;
	OPvec3 Rotation;
	OPvec3 Scale;
};

struct OPmeshDataAnim {
	OPchar* Name;
	OPuint FrameCount;
	OPmat4* Frames; // Based on bone count * FrameCount
};

struct OPmeshData {
    OPvertexLayout vertexLayout;
	OPboundingBox3D bounds;

    // Counts
	ui32 vertexCount;
	ui32 indexCount;
	ui16 hierarchyCount;
	ui16 trackCount;
	ui16 metaCount;

	// Index Size (ui16 or ui32)
	ui32 indexSize;

	void* indices;
	void* vertices;
	i16* hierarchy;
	OPmat4* pose;
	OPmeshDataMeta* meta;
	OPmeshDataAnim* tracks;

    ui32 VertexSize() {
        return vertexLayout.stride;
    }
};
typedef struct OPmeshData OPmeshData;

#include "./Human/include/Rendering/OPvertexBuffer.h"
#include "./Human/include/Rendering/OPindexBuffer.h"
#include "./Human/include/Rendering/OPvertexLayout.h"
#include "./Human/include/Rendering/OPvertexArray.h"

struct OPmeshDesc {
	void* Vertices;
	OPvertexLayout VertexSize;
	ui32 VertexCount;
	void* Indices;
	OPindexSize IndexSize;
	ui32 IndexCount;
};

struct OPmesh;
typedef struct OPmesh OPmesh;

//-----------------------------------------------------------------------------
//   _____ _                   _
//  / ____| |                 | |
// | (___ | |_ _ __ _   _  ___| |_ ___
//  \___ \| __| '__| | | |/ __| __/ __|
//  ____) | |_| |  | |_| | (__| |_\__ \
// |_____/ \__|_|   \__,_|\___|\__|___/
extern OPmesh* OPMESH_ACTIVE; 

struct OPmesh {
	OPvertexBuffer vertexBuffer;
	OPindexBuffer indexBuffer;
	OPvertexArray vertexArray;
	OPvertexLayout vertexLayout;
	OPmeshData* meshData;

	OPboundingBox3D boundingBox;
	ui32 VertexCount;
	void* Vertices;
	OPindexSize IndexSize;
	ui32 IndexCount;
	void* Indicies;
	ui16 MetaCount;
	OPMmeta* Meta;
	ui64 Id;

	OPmesh() {

	}

	OPmesh(OPmeshDesc desc) {
		Init(desc);
	}

	OPmesh(OPvertexLayout vertexLayout) {
		Init(vertexLayout);
	}

	static OPmesh* Create(OPmeshDesc desc);
	static OPmesh* Create(OPvertexLayout vertexLayout);
	OPmesh* Init(OPmeshDesc desc);
	OPmesh* Init(OPvertexLayout vertexLayout);
	void Bind();
	void SetVertexLayout(OPvertexLayout* vertexLayout);
	void UpdateVertexLayout(OPeffect* effect);
	void Build(OPvertexLayout vertexLayout, OPindexSize indSize, OPuint vertCount, OPuint indCount, void* vertices, void* indices);
	void Destroy();
	inline void Free() {
		Destroy();
		OPfree(this);
	}
};

void OPmeshRender();


#endif

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

//-----------------------------------------------------------------------------
//   _____ _                   _
//  / ____| |                 | |
// | (___ | |_ _ __ _   _  ___| |_ ___
//  \___ \| __| '__| | | |/ __| __/ __|
//  ____) | |_| |  | |_| | (__| |_\__ \
// |_____/ \__|_|   \__,_|\___|\__|___/
struct OPmesh {
	OPrenderBuffer VertexBuffer;
	OPrenderBuffer IndexBuffer;
	OPvertexLayout vertexLayout;
	OPmeshData* meshData;

	OPboundingBox3D boundingBox;
	ui32 VertexCount;
	void* Vertices;
	ui32 IndexSize;
	ui32 IndexCount;
	void* Indicies;
	ui16 MetaCount;
	OPMmeta* Meta;
	ui64 Id;
};
typedef struct OPmesh OPmesh;

struct OPmeshDesc {
	void* Vertices;
	ui32 VertexSize;
	ui32 VertexCount;
	void* Indices;
	ui32 IndexSize;
	ui32 IndexCount;
};

//-----------------------------------------------------------------------------
//   _____ _       _           _
//  / ____| |     | |         | |
// | |  __| | ___ | |__   __ _| |___
// | | |_ | |/ _ \| '_ \ / _` | / __|
// | |__| | | (_) | |_) | (_| | \__ \
//  \_____|_|\___/|_.__/ \__,_|_|___/
extern OPmesh* OPMESH_ACTIVE;
extern void* OPMESH_ACTIVE_PTR;

//-----------------------------------------------------------------------------
//  _____                     _____                  _____  _               _   _
// |  __ \                   |  __ \                |  __ \(_)             | | (_)
// | |__) _ __ ___   ______  | |__) _ __ ___   ___  | |  | |_ _ __ ___  ___| |_ ___   _____ ___
// |  ___| '__/ _ \ |______| |  ___| '__/ _ \ / __| | |  | | | '__/ _ \/ __| __| \ \ / / _ / __|
// | |   | | |  __/          | |   | | | (_) | (__  | |__| | | | |  __| (__| |_| |\ V |  __\__ \
// |_|   |_|  \___|          |_|   |_|  \___/ \___| |_____/|_|_|  \___|\___|\__|_| \_/ \___|___/


//-----------------------------------------------------------------------------
// ______                _   _
//|  ____|              | | (_)
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
OPmesh OPmeshCreate();
OPmesh* OPmeshCreate(OPmeshDesc desc);
void OPmeshDestroy(OPmesh* mesh);
void OPmeshBuild(ui32 vertSize, ui32 indSize, OPuint vertCount, OPuint indCount, void* vertices, void* indices);
void OPmeshBind(OPmesh* mesh);

void OPmeshRender();


#endif

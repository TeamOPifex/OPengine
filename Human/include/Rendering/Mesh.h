#ifndef OPENGINE_HUMAN_RENDERER_MESH
#define OPENGINE_HUMAN_RENDERER_MESH
	
#include "./Human/include/Rendering/Buffer.h"
#include "./Math/include/BoundingBox3D.h"
#include "./Human/include/Rendering/Skinning/Animation.h"
#include "./Human/include/Rendering/OPMvertex.h"


//-----------------------------------------------------------------------------
//   _____ _                   _       
//  / ____| |                 | |      
// | (___ | |_ _ __ _   _  ___| |_ ___ 
//  \___ \| __| '__| | | |/ __| __/ __|
//  ____) | |_| |  | |_| | (__| |_\__ \
// |_____/ \__|_|   \__,_|\___|\__|___/
typedef struct{
	OPrenderBuffer VertexBuffer;
	OPrenderBuffer IndexBuffer;
	OPboundingBox3D boundingBox;
	OPskeleton* Skeleton;
	ui32 VertexSize;
	void* Vertices;
	void* Indicies;
	ui16 MetaCount;
	OPMmeta* Meta;
} OPmesh;

//-----------------------------------------------------------------------------
//   _____ _       _           _     
//  / ____| |     | |         | |    
// | |  __| | ___ | |__   __ _| |___ 
// | | |_ | |/ _ \| '_ \ / _` | / __|
// | |__| | | (_) | |_) | (_| | \__ \
//  \_____|_|\___/|_.__/ \__,_|_|___/
extern OPmesh* OPRENDER_CURR_MESH;

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
OPmesh OPrenderCreateMesh();
void OPrenderDestroyMesh(OPmesh* mesh);
void OPrenderBuildMesh(ui32 vertSize, ui32 indSize, ui32 vertCount, ui32 indCount, void* vertices, void* indices);
void OPrenderBindMesh(OPmesh* mesh);

void OPrenderMesh();


#endif
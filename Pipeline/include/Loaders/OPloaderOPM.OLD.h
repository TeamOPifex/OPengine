//#pragma once
//
//#include "./Core/include/OPmemory.h"
//#include "./Core/include/OPlog.h"
//#include "./Data/include/OPfile.h"
//#include "./Data/include/OPhashMap.h"
//#include "./Data/include/OPlinkedList.h"
//#include "./Human/include/Rendering/OPmesh.h"
//#include "./Human/include/Rendering/OPmeshPacked.h"
//#include "./Human/include/Rendering/OPmeshPacker.h"
//#include "./Human/include/Rendering/OPMvertex.h"
//
//struct OPMdataAnim {
//	OPchar* Name;
//	OPuint FrameCount;
//	OPmat4* Frames; // Based on bone count * FrameCount
//};
//
////struct OPMData { 
////	OPvertexLayout vertexLayout;
////	void* vertices;
////	ui32 vertexCount;
////
////	void* indices;
////	ui32 indexCount;
////	OPindexSize indexSize;
////
////	OPboundingBox3D bounds;
////
////	i16* hierarchy;
////	ui16 hierarchyCount;
////	OPmat4* pose;
////
////	ui16 metaCount;
////	OPMmeta* meta;
////
////	ui16 trackCount;
////	OPMdataAnim* tracks;
////};
//
//struct OPMPartNode_def;
//typedef struct OPMPartNode_def OPMPartNode;
//
//struct OPMPartNode_def{
//	OPuint From;
//	OPuint To;
//	OPint ChildCount;
//	OPMPartNode* Children;
//};
//
//void OPMgenerateTangent(OPvec3* tangent, OPMvertex* v1, OPMvertex* v2);
////OPMData OPMloadData(OPstream* str);
//OPint OPMPartitionedLoad(const OPchar* filename, OPmesh** mesh);
//
////OPMData OPMloadData(const OPchar* filename);
////OPint OPMload(OPstream* str, OPmesh** mesh);
//OPint OPMloadPacked(const OPchar* filename, OPmeshPacked** mesh);
////OPint OPMReload(OPstream* str, OPmesh** mesh);
////OPint OPMUnload(void* image);
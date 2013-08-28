// TODO: Have option to use a FILE* to read from disk rather than memory

#pragma once
#ifndef OPEngine_Data_LinkedList
#define OPEngine_Data_LinkedList

#include "./../../Core/include/Types.h"
#include "./../../Core/include/Target.h"
#include "./../../Core/include/DynamicMemory.h"

#if defined(OPIFEX_ANDROID) && defined(__cplusplus)
#include "./../../Core/include/DynamicMemory.h"
#else
//#include "./../../Core/include/Core.h"
#endif

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif
struct OPllNode_def;
typedef struct OPllNode_def OPllNode;

struct OPllNode_def {	
	OPllNode *Prev;
	OPllNode *Next;
	void* Data;
};

typedef struct{
	OPint _size;
	OPllNode* First;
	OPllNode* Last;
} OPlinkedList;

	OPlinkedList* OPllCreate();
	OPint OPllDestroy(OPlinkedList* list);
	OPllNode* OPllInsertFirst(OPlinkedList* list, void* data);
	OPllNode* OPllInsertLast(OPlinkedList* list, void* data);
	void* OPllRemove(OPlinkedList* list, OPllNode* toRemove);
	OPint OPllGetSize(OPlinkedList* list);
#ifdef __cplusplus
};
//-----------------------------------------------------------------------------
//- C++ Definitions -----------------------------------------------------------
namespace OPEngine{
namespace Data{

}
}
#endif
#endif

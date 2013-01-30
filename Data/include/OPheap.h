// TODO: Have option to use a FILE* to read from disk rather than memory

#pragma once
#include "./../../Core/include/Types.h"
#include "./../../Core/include/Target.h"
#include "./../../Core/include/DynamicMemory.h"

#ifndef OPEngine_Data_MinHeap
#define OPEngine_Data_MinHeap

#if defined(OPIFEX_ANDROID) && defined(__cplusplus)

#else
//#include "./../../Core/include/Core.h"
#endif

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif
	typedef struct{
		OPint* _indices;
		OPint _capacity;
		OPint _size;
	} OPminHeap;

	OPminHeap* OPminHeapCreate(OPint capacity);
	OPint OPminHeapDestroy(OPminHeap* heap);
	OPint OPminHeapPush(OPminHeap* heap, OPint value);
	OPint OPminHeapPop(OPminHeap* heap);
	OPint OPminHeapPeek(OPminHeap* heap);
	OPint OPminHeapSize(OPminHeap* heap);

	void _bubbleDown(OPminHeap* heap, OPint i);
	void _bubbleUp(OPint* arr, OPint i);

	OPint _parent(OPint i);
    OPint _leftChild(OPint i);
    OPint _rightChild(OPint i);
	void _swap(OPint* a, OPint* b);
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

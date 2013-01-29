// TODO: Have option to use a FILE* to read from disk rather than memory

#pragma once
#include "./../../Core/include/Types.h"
#include "./../../Core/include/Target.h"

#ifndef OPEngine_Data_MinHeap
#define OPEngine_Data_MinHeap

#if defined(OPIFEX_ANDROID) && defined(__cplusplus)
#include "./../../Core/include/DynamicMemory.h"
#else
//#include "./../../Core/include/Core.h"
#endif

typedef struct{
	OPint* _indices;
	OPint _capacity;
	OPint _size;
} OPminHeap;

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif
	OPminHeap* OPminHeapCreate(OPint capacity);
	OPint OPminHeapDestroy(OPminHeap* heap);
	OPint OPminHeapPush(OPminHeap* heap, OPint value);
	OPint OPminHeapPop(OPminHeap* heap);
	OPint OPminHeapPeek(OPminHeap* heap);
	OPint OPminHeapSize(OPminHeap* heap){
		return heap->_size;
	}

	void _bubbleDown(OPminHeap* heap, OPint i);
	void _bubbleUp(OPint* arr, OPint i);

	OPint _parent(OPint i){
        OPint r = i % 2;

        if (r == 0) return i / 2;
        return (i - 1) / 2;
    }
    OPint _leftChild(OPint i){
        return i * 2;
    }
    OPint _rightChild(OPint i){
        return (i * 2) + 1;
    }
	void _swap(OPint* a, OPint* b){
		OPint temp = *a;
		*a = *b;
		*b = temp;
	}
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

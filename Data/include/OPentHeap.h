#pragma once

#ifndef OPEngine_Data_EntHeap
#define OPEngine_Data_EntHeap

#ifdef __cplusplus
extern "C"
{
#endif
#include "./Core/include/Types.h"
#include "./Core/include/DynamicMemory.h"
#include "./Data/include/OPheap.h"
#include <stdio.h>

//  _____ _                   _       
// / ____| |                 | |      
//| (___ | |_ _ __ _   _  ___| |_ ___ 
// \___ \| __| '__| | | |/ __| __/ __|
// ____) | |_| |  | |_| | (__| |_\__ \
//|_____/ \__|_|   \__,_|\___|\__|___/
//                                                                      
typedef struct{
	void* Entities;
	OPint* InUse;
	OPint MaxIndex;
	OPminHeap Free;
} OPentHeap;

// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/

#define OPentHeapActivate(heap, i){\
	if((*i = OPminHeapPop(&heap->Free)) >= 0){\
		if(*i > heap->MaxIndex - 1){\
			heap->MaxIndex = *i + 1;\
			heap->InUse[heap->MaxIndex] = 1;\
		}\
	}\
}\

#define OPentHeapKill(heap, i){\
	OPint mi = heap.MaxIndex;\
	OPminHeapPush(&heap.Free, i);\
	heap.InUse[heap.MaxIndex] = 0;\
	while(mi){\
		if(heap.InUse[mi] == 1){\
			heap.MaxIndex = mi;\
			break;\
		}\
		mi--;\
	}\
}\

OPuint     OPentHeapSize(OPint entsize, OPint count);
OPentHeap* OPentHeapCreate(void* segPtr, OPint entSize, OPint count);

#ifdef __cplusplus
}
#endif

#endif

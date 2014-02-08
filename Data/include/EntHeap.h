#ifndef OPEngine_Data_EntHeap
#define OPEngine_Data_EntHeap

#ifdef __cplusplus
extern "C"
{
#endif
#include "./Core/include/Types.h"
#include "./Core/include/DynamicMemory.h"
#include "./Data/include/Heap.h"
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

__inline void OPentHeapActivate(OPentHeap* heap, OPint* i){
	if(heap->Free._size){
		if((*i = OPminHeapPop(&heap->Free)) >= 0){
			heap->InUse[*i] = 1;
			if(*i >= heap->MaxIndex - 1){
				heap->MaxIndex = *i + 1;
			}
		}
	}
	else
		*i = -1;
}

__inline void OPentHeapKill(OPentHeap* heap, OPint i){
	OPint inUse = heap->InUse[i];
	if(inUse){
		OPint mi = heap->MaxIndex - 1;
		OPminHeapPush(&heap->Free, i);
		heap->InUse[i] = 0;
		while(!heap->InUse[mi]){
			mi--;
		}
		heap->MaxIndex = mi + 1;
	}
}

OPuint     OPentHeapSize(OPint entsize, OPint count);
OPentHeap* OPentHeapCreate(void* segPtr, OPint entSize, OPint count);

#ifdef __cplusplus
}
#endif

#endif

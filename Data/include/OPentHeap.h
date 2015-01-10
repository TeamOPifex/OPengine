#ifndef OPEngine_Data_EntHeap
#define OPEngine_Data_EntHeap

#ifdef __cplusplus
extern "C"
{
#endif
#include "./Core/include/OPtypes.h"
#include "./Core/include/OPmemory.h"
#include "./Data/include/OPminHeap.h"
#include <stdio.h>

//-----------------------------------------------------------------------------
//  _____ _                   _       
// / ____| |                 | |      
//| (___ | |_ _ __ _   _  ___| |_ ___ 
// \___ \| __| '__| | | |/ __| __/ __|
// ____) | |_| |  | |_| | (__| |_\__ \
//|_____/ \__|_|   \__,_|\___|\__|___/
//                                                                      

typedef struct{
	void*   Entities;
	OPint*  InUse;
	OPint   EntSize;
	OPint   MaxIndex;
	OPuint* Size;
	OPuint  Capacity;
	OPminHeap Free;
} OPentHeap;

#define OPentHeapIsLiving(heap, i)\
if(!heap->InUse[i]) continue;\

//-----------------------------------------------------------------------------
// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/

/* Fills the pointer i with the index of the activated entity in the EntHeap
* @param heap The EntHeap to activate
* @param i Pointer to an OPint, it will contain the activated entity
*/
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

/* Get an indexed entity out of an EntHeap
* @param heap The EntHeap to get an entity
* @param i The index in the EntHeap to get
* @return Pointer to the entity
*/
__inline void* OPentHeapGet(OPentHeap* heap, OPint i){
	return ((ui8*)heap->Entities) + (i * heap->EntSize);
}

/* Kill off an entity in an EntHeap. It will be re-used when OPentHeapActivate is called.
* @param heap The EntHeap to kill of an entity
* @param i Index of the entity in the EntHeap
*/
__inline void OPentHeapKill(OPentHeap* heap, OPint i){
	OPint inUse = heap->InUse[i];
	
	if(inUse){
		OPint mi = heap->MaxIndex - 1;
		OPminHeapPush(&heap->Free, i);
		heap->InUse[i] = 0;
		while(!heap->InUse[mi] && mi >= 0){
			mi--;
		}
		heap->MaxIndex = mi + 1;
	}
}

/* Gets the size required to create an EntHeap
* @param entsize Size of each entity needed for the EntHeap
* @param count Number of entities to manage
* @return size in bytes required
*/
EXPORT_METHOD OPuint     OPentHeapBytes(OPint entsize, OPint count);

/* Creates an EntHeap
* @param segPtr Pointer to a block of memory, use OPentHeapBytes to alloc the right size
* @param entSize Size of each entity in the EntHeap
* @param count Number of entities to manage
* @return A new EntHeap
*/
EXPORT_METHOD OPentHeap* OPentHeapCreate(void* segPtr, OPint entSize, OPint count);

#ifdef __cplusplus
}
#endif

#endif

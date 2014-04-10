#include "Tests/include/helpers.h"
#include "Core/include/MathHelpers.h"
#include "Data/include/EntHeap.h"

OP_TEST_INIT

typedef struct{
	OPint id;
} Dummy;

OPentHeap* heap;

//-----------------------------------------------------------------------------
//    _______        _     ______                      
//   |__   __|      | |   |  ____|                     
//      | | ___  ___| |_  | |__ _   _ _ __   ___ ___   
//      | |/ _ \/ __| __| |  __| | | | '_ \ / __/ __|  
//      | |  __/\__ \ |_  | |  | |_| | | | | (__\__ \_ 
//      |_|\___||___/\__| |_|   \__,_|_| |_|\___|___(_)                                               
OPint shouldCreateHeapWithCapacity(void* args){
	OPuint size = (OPuint)OPrandRange(1, 100);
	void* mem = OPalloc(OPentHeapSize(sizeof(Dummy), size));

	OP_RTMSG("Creating a heap with capacity of %d", size);
	heap = OPentHeapCreate(mem, sizeof(Dummy), size);
	OP_RTMSG("OPint Size: %d", sizeof(OPint));
	OP_RTMSG("HEAP %x", heap);
	OP_RTMSG("Entities: %x", &heap->Entities);
	OP_RTMSG("InUse: %x", &heap->InUse);
	OP_RTMSG("EntSize: %x", &heap->EntSize);
	OP_RTMSG("MaxIndex: %x", &heap->MaxIndex);
	OP_RTMSG("Free: %x", heap->Free);


	OP_RTMSG("Free._indices %x", heap->Free._indices);
	OP_RTMSG("heap.EntSize %x", heap->EntSize);
	OP_RTMSG("Free._capacity %x", heap->Free._capacity);
	OP_RTMSG("Free._size %x", heap->Free._size);
	if(!heap){
		OP_RTMSG("Heap was null!");
		return -1;
	}


	if(heap->Free._indices == NULL){
		OP_RTMSG("Indicies not allocated! %x", heap->Free._indices);
		return -3;
	}
	OP_RTMSG("Indicies allocated!");

	return 0;
}
//-----------------------------------------------------------------------------
OPint pushAndPopShouldReturnSorted(void* args){
	OPint i = 0, last = 0, size = OPminHeapSize(&heap->Free);	
	
	// create a list of random numbers
	for(i = size; i--;){
		OPint index = -1;
		Dummy dummy = {0};
		OPentHeapActivate(heap, &index);
		dummy = *((Dummy*)OPentHeapGet(heap, index));
	}
	
	// last = OPminHeapPop(heap);
	// while(OPminHeapSize(heap) > 0){
	// 	int popped = OPminHeapPop(heap);
	// 	OP_RTMSG("%d < %d", last, popped);
	// 	if(popped < last){
	// 		OP_RTMSG("Popped value was not in the right order!");
	// 		return -2;
	// 	}
	// 	last = popped;
	// }

	return 0;
}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//    _______        _     _____                             
//   |__   __|      | |   |  __ \                            
//      | | ___  ___| |_  | |__) |   _ _ __  _ __   ___ _ __ 
//      | |/ _ \/ __| __| |  _  / | | | '_ \| '_ \ / _ \ '__|
//      | |  __/\__ \ |_  | | \ \ |_| | | | | | | |  __/ |   
//      |_|\___||___/\__| |_|  \_\__,_|_| |_|_| |_|\___|_|   
int main(void){
	OPint result = 0;

	// Run sequence of test function invocations here
	result |= OP_TEST(shouldCreateHeapWithCapacity, "Creation test", NULL);
	result |= OP_TEST(pushAndPopShouldReturnSorted, "Push / Pop test", NULL);

	return (int)result; // value to be read by test script
}

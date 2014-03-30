#include "Tests/include/helpers.h"
#include "Core/include/MathHelpers.h"
#include "Data/include/Heap.h"

OP_TEST_INIT

OPminHeap* heap;

//-----------------------------------------------------------------------------
//    _______        _     ______                      
//   |__   __|      | |   |  ____|                     
//      | | ___  ___| |_  | |__ _   _ _ __   ___ ___   
//      | |/ _ \/ __| __| |  __| | | | '_ \ / __/ __|  
//      | |  __/\__ \ |_  | |  | |_| | | | | (__\__ \_ 
//      |_|\___||___/\__| |_|   \__,_|_| |_|\___|___(_)                                               
OPint shouldCreateHeapWithCapacity(void* args){
	OPuint size = (OPuint)OPrandRange(1, 100);

	OP_RTMSG("Creating a heap with capacity of %d", size);
	heap = OPminHeapCreate(size);

	if(!heap){
		OP_RTMSG("Heap was null!");
		return -1;
	}

	OP_RTMSG("\tCapacity: %u\n\t\tSize: %u", heap->_capacity, heap->_size);
	if(heap->_capacity == size && heap->_size == 0){
		OP_RTMSG("Heap created, expected values found");
	}
	else{
		OP_RTMSG("Heap created, UNEXPECTED values found");
		return -2;
	}


	if(!heap->_indices){
		OP_RTMSG("Indicies not allocated!");
		return -3;
	}
	OP_RTMSG("Indicies allocated!");

	return 0;
}
//-----------------------------------------------------------------------------
OPint pushAndPopShouldReturnSorted(void* args){
	OPint i = 0, last = 0, size = heap->_capacity;	
	
	// create a list of random numbers
	for(i = size; i--;){
		OPminHeapPush(heap, OPrandRange(1, 1000));
	}

	OP_RTMSG("Heap size: %d, expected %d", 
		OPminHeapSize(heap),
		size
	);
	if(OPminHeapSize(heap) != size){
		OP_RTMSG("Size did not match expected");
		return -1;
	}
	
	last = OPminHeapPop(heap);
	while(OPminHeapSize(heap) > 0){
		int popped = OPminHeapPop(heap);
		OP_RTMSG("%d < %d", last, popped);
		if(popped < last){
			OP_RTMSG("Popped value was not in the right order!");
			return -2;
		}
		last = popped;
	}

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
OPint main(void){
	OPint result = 0;

	// Run sequence of test function invocations here
	result |= OP_TEST(shouldCreateHeapWithCapacity, "Creation test", NULL);
	result |= OP_TEST(pushAndPopShouldReturnSorted, "Push / Pop test", NULL);

	return result; // value to be read by test script
}

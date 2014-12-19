#include "Tests/include/helpers.h"
#include "./Data/include/OPallocLinear.h"

OP_TEST_INIT
OPallocLinear* alloc;

typedef struct {
	OPint test;
} Dummy;
Dummy* dummy;

//-----------------------------------------------------------------------------
//    _______        _     ______                      
//   |__   __|      | |   |  ____|                     
//      | | ___  ___| |_  | |__ _   _ _ __   ___ ___   
//      | |/ _ \/ __| __| |  __| | | | '_ \ / __/ __|  
//      | |  __/\__ \ |_  | |  | |_| | | | | (__\__ \_ 
//      |_|\___||___/\__| |_|   \__,_|_| |_|\___|___(_)                                               
OPint shouldCreateAllocLinear(void* args){
	alloc = OPallocLinearCreate(1024);
	return 0;
}

OPint shouldAllocMemoryBlock(void* args){
	dummy = (Dummy*)OPallocLinearAlloc(alloc, sizeof(Dummy));
	return 0;
}

OPint shouldShowCorrectAllocations(void* args){
	if(alloc->_allocCount != 1) return 1;
	if(alloc->_usedMemory != sizeof(Dummy)) return 1;
	if(alloc->_currentPos == alloc->_memStart) return 1;

	OP_RTMSG("Allocations: %d", alloc->_allocCount);
	OP_RTMSG("Used Memory: %d", alloc->_usedMemory);
	OP_RTMSG("Total Memory: %d", alloc->_size);

	return 0;
}

OPint shouldAllocateUpToBounds(void* args){
	for(OPuint i = 0; i < 127; i++) {
		void* test = OPallocLinearAlloc(alloc, sizeof(Dummy));
		if(test == NULL) return 1;
	}

	OP_RTMSG("Allocations: %d", alloc->_allocCount);
	OP_RTMSG("Used Memory: %d", alloc->_usedMemory);
	OP_RTMSG("Total Memory: %d", alloc->_size);

	return 0;
}

OPint shouldNotAllocateOutOfBounds(void* args){
	void* test = OPallocLinearAlloc(alloc, sizeof(Dummy));
	if(test != NULL) return 1;

	OP_RTMSG("Allocations: %d", alloc->_allocCount);
	OP_RTMSG("Used Memory: %d", alloc->_usedMemory);
	OP_RTMSG("Total Memory: %d", alloc->_size);

	return 0;
}

OPint shouldClear(void* args){
	OPallocLinearClear(alloc);
	if(alloc->_allocCount != 0) return 1;
	if(alloc->_usedMemory != 0) return 1;
	if(alloc->_currentPos != alloc->_memStart) return 1;

	OP_RTMSG("Allocations: %d", alloc->_allocCount);
	OP_RTMSG("Used Memory: %d", alloc->_usedMemory);
	OP_RTMSG("Total Memory: %d", alloc->_size);

	return 0;
}

OPint shouldDestroy(void* args){
	OPallocLinearDestroy(alloc);

	return 0;
}

/*
//-----------------------------------------------------------------------------
//    _______        _     _____                             
//   |__   __|      | |   |  __ \                            
//      | | ___  ___| |_  | |__) |   _ _ __  _ __   ___ _ __ 
//      | |/ _ \/ __| __| |  _  / | | | '_ \| '_ \ / _ \ '__|
//      | |  __/\__ \ |_  | | \ \ |_| | | | | | | |  __/ |   
//      |_|\___||___/\__| |_|  \_\__,_|_| |_|_| |_|\___|_|   
*/
int main(void){
	int result = 0;

	// Run sequence of test function invocations here
	result |= OP_TEST(shouldCreateAllocLinear, "shouldCreateAllocLinear", NULL);
	if(result) return result;

	result |= OP_TEST(shouldAllocMemoryBlock, "shouldAllocMemoryBlock", NULL);
	result |= OP_TEST(shouldShowCorrectAllocations, "shouldShowCorrectAllocations", NULL);
	result |= OP_TEST(shouldAllocateUpToBounds, "shouldAllocateUpToBounds", NULL);
	result |= OP_TEST(shouldNotAllocateOutOfBounds, "shouldNotAllocateOutOfBounds", NULL);
	result |= OP_TEST(shouldClear, "shouldClear", NULL);
	result |= OP_TEST(shouldDestroy, "shouldDestroy", NULL);

	return result; // value to be read by test script
}
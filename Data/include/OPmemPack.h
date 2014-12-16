#ifndef OPEngine_Data_MemPack
#define OPEngine_Data_MemPack

#ifdef __cplusplus
extern "C"
{
#endif

#include "./Core/include/Types.h"
#include "./Core/include/DynamicMemory.h"

//  _____ _                   _       
// / ____| |                 | |      
//| (___ | |_ _ __ _   _  ___| |_ ___ 
// \___ \| __| '__| | | |/ __| __/ __|
// ____) | |_| |  | |_| | (__| |_\__ \
//|_____/ \__|_|   \__,_|\___|\__|___/
//                                                                      

typedef struct{
	void (*Alloc)(void* segmentPtr, OPuint count);
	void (*Dealloc)(void* data);
	OPuint (*Size)(OPuint count);
	void* Data;
} OPmemDesc;

// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/

/* Allocates a block of memory to be used in segements
 * @param segments descriptors
 * @param segCount number of segments
 * @param n size to allocate
 * @return pointer to the allocated block of memory
*/
void* OPmemPackAlloc(OPmemDesc* segments, OPuint segCount, OPint n);

void OPmemPackDealloc(void* Data, OPmemDesc* segments, OPint segCount, OPint n);

#ifdef __cplusplus
}
#endif

#endif

#include "Core.h"

void* OPalloc(ui32 bytes){
	return malloc(bytes);
}

void OPfree(void* ptr){
	if(ptr) free(ptr);
}

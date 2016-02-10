#ifndef OPENGINE_DATA_OPALLOCATOR
#define OPENGINE_DATA_OPALLOCATOR

#include "./Core/include/OPtypes.h"

// Ensures allocators are 4 byte aligned
inline ui8 OPallocAlignAdjustment(const void* address, ui8 alignment)
{
    ui8 adjustment =  alignment - ( address & (alignment-1) );

    if(adjustment == alignment)
        return 0; //already aligned

    return adjustment;
}

#endif

#pragma once

#include "./Core/include/OPtypes.h"

// Ensures allocators are 4 byte aligned
inline ui8 OPallocAlignAdjustment(const void* address, ui8 alignment)
{
    ui8 adjustment =  alignment - ( (OPint)address & (alignment-1) );

    if(adjustment == alignment)
        return 0; //already aligned

    return adjustment;
}

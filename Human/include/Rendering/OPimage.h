#pragma once

struct OPimage;
typedef struct OPimage OPimage;

#include "./Core/include/OPtypes.h"

/**
 * OPimage
 * Describes an image
 *
 */
struct OPimage {
    ui32 Width;
    ui32 Height;
    ui8 Components;
    ui8* Data;
};
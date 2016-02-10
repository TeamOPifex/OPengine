#ifndef OPENGINE_HUMAN_RENDERING_IMAGE
#define OPENGINE_HUMAN_RENDERING_IMAGE

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

#endif
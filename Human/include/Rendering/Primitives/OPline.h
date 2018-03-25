#pragma once

#include "./Human/include/Rendering/OPmodel.h"

struct OPline {
    OPmodel model;
    void Init(OPvec3* points, ui32 count);
    void Update(OPvec3* points, ui32 count);
    void Draw();
};
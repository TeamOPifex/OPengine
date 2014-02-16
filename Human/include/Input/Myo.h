#ifndef OPENGINE_HUMAN_INPUT_OCULUS
#define OPENGINE_HUMAN_INPUT_OCULUS

#include "./Core/include/Types.h"

void OPmyoConnect();
void OPmyoUpdate();

f32 OPmyoRoll();
f32 OPmyoPitch();
f32 OPmyoYaw();
ui32 OPmyoPose();

#endif
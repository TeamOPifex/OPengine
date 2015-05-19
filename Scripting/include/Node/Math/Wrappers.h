#ifndef OPENGINE_SCRIPTING_NODE_MATH_WRAPPERS
#define OPIFEXENGINE_WRAPPER_H

#include "./Scripting/include/Node/OPscriptNodeHelper.h"
#include "./Math/include/OPmat4.h"

void OPscriptNodeWrapperMath(Handle<Object> target);

void OPvec2Wrapper(Handle<Object> exports);
void OPvec3Wrapper(Handle<Object> exports);
void OPmat4Wrapper(Handle<Object> exports);

#endif //OPIFEXENGINE_WRAPPER_H

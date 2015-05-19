#ifndef OPENGINE_SCRIPTING_NODE_MATH_WRAPPERS
#define OPENGINE_SCRIPTING_NODE_MATH_WRAPPERS

#include "./Scripting/include/Node/OPscriptNodeHelper.h"

void OPscriptNodeWrapperMath(Handle<Object> target);

void OPvec2Wrapper(Handle<Object> exports);
void OPvec3Wrapper(Handle<Object> exports);
void OPmat4Wrapper(Handle<Object> exports);

#endif
#ifndef OPENGINE_SCRIPTING_NODE_MATH_WRAPPERS
#define OPENGINE_SCRIPTING_NODE_MATH_WRAPPERS

#ifdef OPIFEX_OPTION_NODEJS

#include "./Scripting/include/JavaScript/OPjavaScriptHelper.h"

void OPscriptNodeWrapperMath(Handle<Object> target);

void OPvec2Wrapper(Handle<Object> exports);
void OPvec3Wrapper(Handle<Object> exports);
void OPvec4Wrapper(Handle<Object> exports);
void OPmat4Wrapper(Handle<Object> exports);

#include "./Math/include/OPvec3.h"

void OPvec3WrapperSetup(Handle<Object> result, OPvec3* ptr);

#endif

#endif
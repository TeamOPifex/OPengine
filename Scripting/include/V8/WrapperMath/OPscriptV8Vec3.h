#ifndef OPENGINE_SCRIPTING_V8_WRAPPERMATH_VEC3
#define OPENGINE_SCRIPTING_V8_WRAPPERMATH_VEC3

#include "./OPengineDefines.h"

#ifdef OPIFEX_OPTION_V8

#include "./Math/include/OPvec3.h"

//Handle<Object> OPscriptV8Create(OPvec2* val);
Handle<Object> OPscriptV8Create(OPvec3* val);
// Handle<Object> OPscriptV8Create(OPvec4* val);
// Handle<Object> OPscriptV8Create(OPmat3* val);
// Handle<Object> OPscriptV8Create(OPmat4* val);
// Handle<Object> OPscriptV8Create(OPquat* val);

#endif

#endif
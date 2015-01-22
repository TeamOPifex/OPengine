#ifndef OPENGINE_SCRIPTING_V8_WRAPPERHUMAN
#define OPENGINE_SCRIPTING_V8_WRAPPERHUMAN

#include "./OPengineDefines.h"

#ifdef OPIFEX_OPTION_V8

#include "./Scripting/include/V8/OPscriptV8Types.h"

void OPscriptV8WrapperHuman(Handle<Object> target);
#include "./Math/include/OPvec3.h"
Handle<Object> OPscriptV8Create(OPvec3* val);
#endif

#endif
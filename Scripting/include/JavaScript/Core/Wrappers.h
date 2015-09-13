#ifndef OPENGINE_SCRIPTING_NODE_CORE_WRAPPERS
#define OPENGINE_SCRIPTING_NODE_CORE_WRAPPERS

#include "./Scripting/include/JavaScript/OPjavaScriptHelper.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

void OPscriptNodeWrapperCore(Handle<Object> target);

void OPtimerWrapper(Handle<Object> exports);
void OPdefinedWrapper(Handle<Object> exports);

#endif

#endif
#ifndef OPENGINE_SCRIPTING_NODE_CORE_WRAPPERS
#define OPENGINE_SCRIPTING_NODE_CORE_WRAPPERS

#ifdef OPIFEX_OPTION_NODEJS

#include "./Scripting/include/JavaScript/OPjavaScriptHelper.h"

void OPscriptNodeWrapperCore(Handle<Object> target);

void OPtimerWrapper(Handle<Object> exports);
void OPdefinedWrapper(Handle<Object> exports);

#endif

#endif
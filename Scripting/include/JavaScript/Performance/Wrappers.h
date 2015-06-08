#ifndef OPENGINE_SCRIPTING_NODE_PERFORMANCE_WRAPPERS
#define OPENGINE_SCRIPTING_NODE_PERFORMANCE_WRAPPERS

#ifdef OPIFEX_OPTION_NODEJS

#include "./Scripting/include/JavaScript/OPjavaScriptHelper.h"

void OPscriptNodeWrapperPerformance(Handle<Object> target);

void OPphysXWrapper(Handle<Object> exports);
void OPphysXControllerWrapper(Handle<Object> exports);
void OPphysXSceneWrapper(Handle<Object> exports);

#endif

#endif

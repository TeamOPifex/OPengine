#pragma once

#include "./Scripting/include/JavaScript/OPjavaScriptHelper.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

void OPscriptNodeWrapperPerformance(Handle<Object> target);

void OPphysXWrapper(Handle<Object> exports);
void OPphysXControllerWrapper(Handle<Object> exports);
void OPphysXSceneWrapper(Handle<Object> exports);

#endif

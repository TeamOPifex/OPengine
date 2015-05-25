#ifndef OPENGINE_SCRIPTING_NODE_PIPELINE_WRAPPERS
#define OPENGINE_SCRIPTING_NODE_PIPELINE_WRAPPERS

#ifdef OPIFEX_OPTION_NODEJS

#include "./Scripting/include/JavaScript/OPjavaScriptHelper.h"

void OPscriptNodeWrapperPipeline(Handle<Object> target);

void OPtexture2DWrapper(Handle<Object> exports);
void OPloaderWrapper(Handle<Object> exports);

#endif

#endif
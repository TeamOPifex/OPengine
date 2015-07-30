#ifndef OPENGINE_SCRIPTING_NODE_PIPELINE_WRAPPERS
#define OPENGINE_SCRIPTING_NODE_PIPELINE_WRAPPERS

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Scripting/include/JavaScript/OPjavaScriptHelper.h"

void OPscriptNodeWrapperPipeline(Handle<Object> target);

void OPloaderVoxelsWrapper(Handle<Object> exports);
void OPvoxelGeneratorWrapper(Handle<Object> exports);
void OPtexture2DWrapper(Handle<Object> exports);
void OPloaderWrapper(Handle<Object> exports);
void OPsprite2DWrapper(Handle<Object> exports);
void OPspriteSystemWrapper(Handle<Object> exports);
void OPfmodWrapper(Handle<Object> exports);

#endif

#endif

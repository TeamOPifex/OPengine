#ifndef OPENGINE_SCRIPTING_NODE_PIPELINE_WRAPPERS
#define OPIFEXENGINE_WRAPPER_H

#include "./Scripting/include/Node/OPscriptNodeHelper.h"
#include "./Human/include/Rendering/OPcam.h"

void OPscriptNodeWrapperPipeline(Handle<Object> target);

void OPtexture2DWrapper(Handle<Object> exports);
void OPloaderWrapper(Handle<Object> exports);

#endif //OPIFEXENGINE_WRAPPER_H

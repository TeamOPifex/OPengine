#ifndef OPENGINE_SCRIPTING_NODE_CORE_WRAPPERS
#define OPIFEXENGINE_WRAPPER_H

#include "./Scripting/include/Node/OPscriptNodeHelper.h"
#include "./Human/include/Rendering/OPcam.h"

void OPscriptNodeWrapperCore(Handle<Object> target);

void OPtimerWrapper(Handle<Object> exports);
void OPdefinedWrapper(Handle<Object> exports);

#endif //OPIFEXENGINE_WRAPPER_H

#ifndef OPENGINE_SCRIPTING_NODE_HUMAN_WRAPPERS
#define OPIFEXENGINE_WRAPPER_H

#include "./Scripting/include/Node/OPscriptNodeHelper.h"
#include "./Human/include/Rendering/OPcam.h"

void OPscriptNodeWrapperHuman(Handle<Object> target);

void OPcamWrapper(Handle<Object> exports);
void OPcubeWrapper(Handle<Object> exports);
void OPeffectWrapper(Handle<Object> exports);
void OPfontWrapper(Handle<Object> exports);
void OPgamePadWrapper(Handle<Object> exports);
void OPkeyboardWrapper(Handle<Object> exports);
void OPmaterialWrapper(Handle<Object> exports);
void OPmeshWrapper(Handle<Object> exports);
void OPrenderWrapper(Handle<Object> exports);
void OPtextureWrapper(Handle<Object> exports);

void OPATTRWrapper(Handle<Object> exports);

#endif //OPIFEXENGINE_WRAPPER_H

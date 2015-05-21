#ifndef OPENGINE_SCRIPTING_NODE_HUMAN_WRAPPERS
#define OPENGINE_SCRIPTING_NODE_HUMAN_WRAPPERS

#include "./Scripting/include/JavaScript/OPjavaScriptHelper.h"

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

#endif
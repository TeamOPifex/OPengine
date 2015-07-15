#ifndef OPENGINE_SCRIPTING_NODE_HUMAN_WRAPPERS
#define OPENGINE_SCRIPTING_NODE_HUMAN_WRAPPERS

#include "./Scripting/include/JavaScript/OPjavaScriptHelper.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Human/Human.h"

void OPscriptNodeWrapperHuman(Handle<Object> target);

void OPcamFreeFlightWrapper(Handle<Object> exports);
void OPcamWrapper(Handle<Object> exports);
void OPcubeWrapper(Handle<Object> exports);
void OPeffectWrapper(Handle<Object> exports);
void OPfontWrapper(Handle<Object> exports);
void OPfontManagerWrapper(Handle<Object> exports);
void OPfontRenderWrapper(Handle<Object> exports);
void OPgamePadWrapper(Handle<Object> exports);
void OPkeyboardWrapper(Handle<Object> exports);
void OPmaterialWrapper(Handle<Object> exports);
void OPmeshWrapper(Handle<Object> exports);
void OPmodelWrapper(Handle<Object> exports);
void OPrenderWrapper(Handle<Object> exports);
void OPtextureWrapper(Handle<Object> exports);
void OPskeletonWrapper(Handle<Object> exports);

void OPATTRWrapper(Handle<Object> exports);

void OPfontManagerWrapperCreate(Handle<Object> result, OPfontManager* ptr);
void OPcamWrapperCreate(Handle<Object> result, OPcam* ptr);
void OPmeshWrapperCreate(Handle<Object> result, OPmesh* mesh);
Handle<Object> OPsekeletonWrapperCreate(OPskeleton* skeleton);

#endif

#endif

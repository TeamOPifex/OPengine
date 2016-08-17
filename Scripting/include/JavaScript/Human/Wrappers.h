#pragma once

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
void OPskeletonAnimationWrapper(Handle<Object> exports);
void OPframeBufferWrapper(Handle<Object> exports);
void OPrenderCommandBucketWrapper(Handle<Object> exports);
void OPcommandDrawIndexedWrapper(Handle<Object> exports);
void OPvertexLayoutWrapper(Handle<Object> exports);

void OPATTRWrapper(Handle<Object> exports);

JS_EASY_WRAP_PTR(OPcam)
JS_EASY_WRAP_PTR(OPfontManager)
JS_EASY_WRAP_PTR(OPmesh)
JS_EASY_WRAP_PTR(OPmodel)
JS_EASY_WRAP_PTR(OPmodelTextured)

#define JS_WRAP_OPCAM(EXTERNAL,NAME,PTR) JS_WRAP_(OPcamWrapper, EXTERNAL,NAME,PTR)

#endif
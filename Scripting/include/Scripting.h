#ifndef OPIFEX_SCRIPTING
#define OPIFEX_SCRIPTING

#include "./Scripting/include/wrappers/V8Helper.h"
#include "./Human/include/Utilities/LoaderOPS.h"
#include "./Data/include/Stream.h"

#ifdef OPIFEX_V8

#include "v8.h"
#include "./include/libplatform/libplatform.h"
using namespace v8;

extern V8isolate* isolate;
extern void(*CustomWrapper)(V8isolate* isolate, V8ObjectGlobal target);
#endif

void OPscriptInit();
void OPscriptCompileAndRun(OPscript* script);
void OPscriptCompileAndRunStream(OPstream* stream);
void OPscriptLog(const char* data); 
#endif
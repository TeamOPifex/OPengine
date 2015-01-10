#ifndef OPIFEX_SCRIPTING
#define OPIFEX_SCRIPTING

#include "./Scripting/include/wrappers/V8Helper.h"
#include "./Scripting/include/OPloaderOPS.h"
#include "./Data/include/OPstream.h"

#ifdef OPIFEX_V8

#include "v8.h"
#include "./include/libplatform/libplatform.h"
using namespace v8;

extern V8isolate* isolate;
extern void(*CustomWrapper)(V8isolate* isolate, V8ObjectGlobal target);
#endif

EXPORT_METHOD void OPscriptInit();
EXPORT_METHOD void OPscriptCompileAndRun(OPscript* script);
EXPORT_METHOD void OPscriptCompileAndRunStream(OPstream* stream);
EXPORT_METHOD void OPscriptLog(const char* data);
#endif
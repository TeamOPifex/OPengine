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

typedef struct {
#ifdef OPIFEX_V8
	Persistent<Script, CopyablePersistentTraits<Script>> result;
	Persistent<Context, CopyablePersistentTraits<Context>> context;
	Persistent<ObjectTemplate, CopyablePersistentTraits<ObjectTemplate>> global;
#endif
} OPscriptCompiled;

void OPscriptInit();
OPscriptCompiled OPscriptCompile(OPscript* script);
void OPscriptRunFunc(OPscriptCompiled* scriptCompiled, OPchar* name, OPint count, ...);
void OPscriptRun(OPscriptCompiled* scriptCompiled);
void OPscriptCompileAndRun(OPscript* script);
void OPscriptCompileAndRunStream(OPstream* stream);
void OPscriptLog(const char* data); 
#endif
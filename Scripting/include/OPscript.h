#ifndef OPIFEX_SCRIPTING
#define OPIFEX_SCRIPTING

#include "./Scripting/include/wrappers/V8Helper.h"
#include "./Scripting/include/OPloaderOPS.h"
#include "./Data/include/OPstream.h"

#ifdef OPIFEX_V8

#include "v8.h"
#include "./include/libplatform/libplatform.h"
using namespace v8;

extern void(*CustomWrapper)(V8isolate* isolate, V8ObjectGlobal target);
#endif

typedef struct {
	OPscript* source;
#ifdef OPIFEX_V8
	Persistent<Script, CopyablePersistentTraits<Script>> result;
	Persistent<Context, CopyablePersistentTraits<Context>> context;
	Persistent<ObjectTemplate, CopyablePersistentTraits<ObjectTemplate>> global;
#endif
} OPscriptCompiled;

OPint OPscriptCompile(OPscriptCompiled* scriptCompiled, OPscript* script, Persistent<Context, CopyablePersistentTraits<Context>>* existingContext);
OPscriptValuePersistent OPscriptRunFunc(OPscriptCompiled* scriptCompiled, OPchar* name, OPint count, OPscriptValuePersistent* args);
void OPscriptRun(OPscriptCompiled* scriptCompiled);
void OPscriptCompileAndRun(OPscript* script);
void OPscriptCompileAndRunStream(OPstream* stream);
void OPscriptLog(const char* data); 

inline OPint OPscriptCompile(OPscriptCompiled* scriptCompiled, OPscript* script) {
	return OPscriptCompile(scriptCompiled, script, NULL);
}
inline OPscriptValuePersistent OPscriptRunFunc(OPscriptCompiled* scriptCompiled, OPchar* name) {
	return OPscriptRunFunc(scriptCompiled, name, 0, NULL);
}
#endif
#ifndef OPENGINE_SCRIPTING_V8_OPSCRIPTV8
#define OPENGINE_SCRIPTING_V8_OPSCRIPTV8

#include "./OPengineDefines.h"

#ifdef OPIFEX_OPTION_V8

#include "./Scripting/include/OPloaderOPS.h"

#include "v8.h"
using namespace v8;

typedef Isolate OPscriptV8Isolate;
typedef Persistent<Script, CopyablePersistentTraits<Script> > OPscriptV8PersistentScript;
typedef Persistent<Context, CopyablePersistentTraits<Context> > OPscriptV8PersistentContext;
typedef Persistent<ObjectTemplate, CopyablePersistentTraits<ObjectTemplate> > OPscriptV8PersistentGlobalTemplate;
typedef Persistent<Value, CopyablePersistentTraits<Value> > OPscriptV8PersistentValue;
typedef Persistent<Object, CopyablePersistentTraits<Object> > OPscriptV8PersistentObject;

struct OPscriptV8Compiled {
	OPscript* Source;
	OPscriptV8PersistentScript Script;
	OPscriptV8PersistentContext Context;
};

extern OPscriptV8Isolate* OPSCRIPTV8_ISOLATE;
extern void(*OPSCRIPTV8_WRAPPER)(Handle<ObjectTemplate>);
extern OPint(*OPSCRIPTV8_REQUIRE)(FunctionCallbackInfo<Value>);

void OPscriptV8Init();
OPint OPscriptV8Compile(OPscriptV8Compiled* compiled, OPscript* script, OPscriptV8PersistentContext* context);
void OPscriptV8Update(OPscriptV8Compiled* scriptCompiled);
Local<Value> OPscriptV8Run(OPscriptV8Compiled* scriptCompiled);
OPscriptV8PersistentValue OPscriptV8Run(OPscriptV8Compiled* scriptCompiled, OPchar* name);
OPscriptV8PersistentValue OPscriptV8Run(OPscriptV8Compiled* scriptCompiled, OPchar* name, OPint count, Persistent<Value>* args);

#ifdef _DEBUG
	#define OPSCRIPTV8_UPDATE(compiled) OPscriptV8Update(compiled);
#else
	#define OPSCRIPTV8_UPDATE(compiled)
#endif

#endif

#endif
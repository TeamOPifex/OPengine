#ifndef OPENGINE_SCRIPTING_V8_OPSCRIPTV8TYPES
#define OPENGINE_SCRIPTING_V8_OPSCRIPTV8TYPES

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

#endif

#endif
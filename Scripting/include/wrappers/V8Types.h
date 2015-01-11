#ifndef OPIFEX_SCRIPTING_WRAPPERS_V8_TYPES
#define OPIFEX_SCRIPTING_WRAPPERS_V8_TYPES

#include "./Core/include/OPtypes.h"

#ifndef OPIFEX_V8
typedef OPint OPscriptValuePersistent;
typedef OPint OPscriptPersistentContext;
#endif

#ifdef OPIFEX_V8

#include "v8.h"
using namespace v8;


enum OPscriptTypes {
	OPscript_CAMERA = 0,
	OPscript_VEC3,
	OPscript_MAT4,
	OPscript_MESH,
	OPscript_TIMER,
	OPscript_PHYSICS_SCENE,
	OPscript_PHYSICS_ACTOR,
	OPscript_PHYSICS_STATIC,
	OPscript_PHYSICS_DYNAMIC,
	OPscript_AUDIO_EMITTER,
	OPscript_FRAME_BUFFER
};

#include "./Core/include/OPtypes.h"

typedef Handle<ObjectTemplate> V8ObjectTemplate;
typedef Handle<Object> V8Object;
typedef Handle<Value> OPscriptValue;
typedef Persistent<Value> OPscriptValuePersistent;
typedef Persistent<Context, CopyablePersistentTraits<Context>> OPscriptPersistentContext
#ifdef OPIFEX_NODEJS

typedef Arguments V8Args;
typedef Handle<Value> V8Return;
typedef i8* V8isolate;
typedef V8Object V8ObjectGlobal;
typedef InvocationCallback V8Function;
typedef HandleScope V8Scope;

#else

typedef FunctionCallbackInfo<Value> V8Args;
typedef void V8Return;
typedef Isolate V8isolate;
typedef V8ObjectTemplate V8ObjectGlobal;
typedef FunctionCallback V8Function;
typedef i8 V8Scope;

#endif

#endif

#endif
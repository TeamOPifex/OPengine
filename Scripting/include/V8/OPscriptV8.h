#ifndef OPENGINE_SCRIPTING_V8_OPSCRIPTV8
#define OPENGINE_SCRIPTING_V8_OPSCRIPTV8

#include "./OPengineDefines.h"

#ifdef OPIFEX_OPTION_V8

#include "./Scripting/include/V8/OPscriptV8Types.h"

extern OPscriptV8Isolate* OPSCRIPTV8_ISOLATE;
extern void(*OPSCRIPTV8_WRAPPER)(Handle<ObjectTemplate>);
extern OPint(*OPSCRIPTV8_REQUIRE)(FunctionCallbackInfo<Value>);

void OPscriptV8Init();
OPint OPscriptV8Compile(OPscriptV8Compiled* compiled, OPscript* script);
OPint OPscriptV8Compile(OPscriptV8Compiled* compiled, OPscript* script, OPscriptV8PersistentContext* context);
void OPscriptV8Update(OPscriptV8Compiled* scriptCompiled);
Local<Value> OPscriptV8Run(OPscriptV8Compiled* scriptCompiled);
OPscriptV8PersistentValue OPscriptV8Run(OPscriptV8Compiled* scriptCompiled, OPchar* name);
OPscriptV8PersistentValue OPscriptV8Run(OPscriptV8Compiled* scriptCompiled, OPchar* name, OPint count, OPscriptV8PersistentValue* args);
OPscriptV8PersistentObject OPscriptV8CreateObject();

#ifdef _DEBUG
	#define OPSCRIPTV8_UPDATE(compiled) OPscriptV8Update(compiled);
#else
	#define OPSCRIPTV8_UPDATE(compiled)
#endif

inline Handle<String> OPscriptV8GetString(const OPchar* val) {
	return String::NewFromUtf8(OPSCRIPTV8_ISOLATE, val);
}
inline Handle<Value> OPscriptV8GetNumber(OPint number) {
	return Number::New(OPSCRIPTV8_ISOLATE, number);
}
inline void OPscriptV8SetFunction(Handle<Object> obj, const OPchar* name, FunctionCallback function) {
	Local<Function> func = Function::New(OPSCRIPTV8_ISOLATE, function);
	obj->Set(OPscriptV8GetString(name), func);
}

inline void OPscriptV8SetObject(Handle<Object> target, const OPchar* name, Handle<Object> val) {
	target->Set(OPscriptV8GetString(name), val);
}

#endif

#endif
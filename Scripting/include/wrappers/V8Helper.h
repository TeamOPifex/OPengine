#ifndef OPIFEX_SCRIPTING_WRAPPERS_V8
#define OPIFEX_SCRIPTING_WRAPPERS_V8

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

#include "./Core/include/Types.h"

typedef Handle<ObjectTemplate> V8ObjectT;
typedef Handle<Object> V8Object;

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
typedef V8ObjectT V8ObjectGlobal;
typedef FunctionCallback V8Function;
typedef i8 V8Scope;

#endif

extern V8isolate* isolate;

#ifdef OPIFEX_NODEJS

inline V8Return SetReturn(const V8Args& args, V8Scope* scope, Handle<Value> val) {
	return scope->Close(val);
}

inline V8Object CreateObject(V8isolate* isolate) {
	return Object::New();
}

inline V8ObjectGlobal CreateObjectG(V8isolate* isolate) {
	return Object::New();
}

inline V8ObjectT CreateObjectT(V8isolate* isolate) {
	return ObjectTemplate::New();
}

inline Handle<String> GetString(V8isolate* isolate, i8* name) {
	return String::New(name);
}

inline Handle<Value> GetNull(V8isolate* isolate) {
	return Null();
}

inline Handle<Array> CreateArray(V8isolate* isolate) {
	return Array::New();
}

inline Handle<Value> GetNumber(V8isolate* isolate, OPint number) {
	return Number::New(number);
}

inline Handle<Value> GetBool(V8isolate* isolate, bool val) {
	return Boolean::New(val);
}

inline Handle<Value> GetNumberF32(V8isolate* isolate, f32 number) {
	return Number::New(number);
}

inline void SetFunction(V8isolate* isolate, V8Object obj, i8* name, V8Function function) {
	obj->Set(GetString(isolate, name), FunctionTemplate::New(function)->GetFunction());
}

inline void SetFunctionT(V8isolate* isolate, V8ObjectT obj, i8* name, V8Function function) {
	obj->Set(GetString(isolate, name), FunctionTemplate::New(function)->GetFunction());
}

inline void SetFunctionG(V8isolate* isolate, V8ObjectGlobal obj, i8* name, V8Function function) {
	obj->Set(GetString(isolate, name), FunctionTemplate::New(function)->GetFunction());
}

inline void SetValue(V8isolate* isolate, V8Object obj, i8* name, Handle<Value> set) {
	obj->Set(GetString(isolate, name), set);
}

inline void SetValueG(V8isolate* isolate, V8ObjectGlobal obj, i8* name, Handle<Value> set) {
	obj->Set(GetString(isolate, name), set);
}

inline void SetObject(V8isolate* isolate, V8Object obj, i8* name, V8Object set) {
	obj->Set(GetString(isolate, name), set);
}

inline void SetObjectT(V8isolate* isolate, V8ObjectT obj, i8* name, V8ObjectT set) {
	obj->Set(GetString(isolate, name), set);
}

inline void SetObjectG(V8isolate* isolate, V8ObjectGlobal obj, i8* name, V8ObjectGlobal set) {
	obj->Set(GetString(isolate, name), set);
}

#else

inline V8Return SetReturn(const V8Args& args, V8Scope* scope, Handle<Value> val) {
	return args.GetReturnValue().Set(val);
}

inline V8Object CreateObject(V8isolate* isolate) {
	return Object::New(isolate);
}

inline V8ObjectT CreateObjectT(V8isolate* isolate) {
	return ObjectTemplate::New(isolate);
}

inline V8ObjectGlobal CreateObjectG(V8isolate* isolate) {
	return ObjectTemplate::New(isolate);
}

inline Handle<Array> CreateArray(V8isolate* isolate) {
	return Array::New(isolate);
}

inline Handle<String> GetString(V8isolate* isolate, const OPchar* name) {
	return String::NewFromUtf8(isolate, name);
}

inline Handle<Value> GetNull(V8isolate* isolate) {
	return Null(isolate);
}

inline Handle<Value> GetNumber(V8isolate* isolate, OPint number) {
	return Number::New(isolate, number);
}

inline Handle<Value> GetBool(V8isolate* isolate, bool val) {
	return Boolean::New(isolate, val);
}

inline Handle<Value> GetNumberF32(V8isolate* isolate, f32 number) {
	return Number::New(isolate, number);
}

inline void SetFunction(V8isolate* isolate, V8Object obj, const OPchar* name, V8Function function) {
	Local<Function> func = Function::New(isolate, function);
	obj->Set(GetString(isolate, name), func);
}

inline void SetFunctionT(V8isolate* isolate, V8ObjectT obj, const OPchar* name, V8Function function) {
	obj->Set(GetString(isolate, name), FunctionTemplate::New(isolate, function));
}

inline void SetFunctionG(V8isolate* isolate, V8ObjectGlobal obj, const OPchar* name, V8Function function) {
	obj->Set(GetString(isolate, name), FunctionTemplate::New(isolate, function));
}

inline void SetValue(V8isolate* isolate, V8Object obj, const OPchar* name, Handle<Value> set) {
	obj->Set(GetString(isolate, name), set);
}

inline void SetValueG(V8isolate* isolate, V8ObjectGlobal obj, const OPchar* name, Handle<Value> set) {
	obj->Set(GetString(isolate, name), set);
}

inline void SetObject(V8isolate* isolate, V8Object obj, const OPchar* name, V8Object set) {
	obj->Set(GetString(isolate, name), set);
}

inline void SetObjectT(V8isolate* isolate, V8ObjectT obj, const OPchar* name, V8ObjectT set) {
	obj->Set(GetString(isolate, name), set);
}

inline void SetObjectG(V8isolate* isolate, V8ObjectGlobal obj, const OPchar* name, V8ObjectGlobal set) {
	obj->Set(GetString(isolate, name), set);
}

inline f32 GetNumber(V8isolate* isolate, V8Object obj, const OPchar* name, f32 def) {
	Handle<String> n = GetString(isolate, name);
	if (obj->HasRealNamedProperty(n)) {
		return obj->Get(n)->NumberValue();
	}
	return def;
}

#endif


void LogProperties(Handle<Object> obj); 
bool IsObject(const V8Args& args, V8isolate* isolate, i32 position, OPscriptTypes type);
V8Object CreateTypedObject(V8isolate* isolate, void* Id, OPscriptTypes type);
void* GetFirstPointer(const V8Args& args, V8isolate* isolate, i32* result, OPscriptTypes type);
void* GetCallingPointer(const V8Args& args, V8isolate* isolate);
void* GetArgPointer(const V8Args& args, V8isolate* isolate, i32 position);
void* GetPointer(const V8Args& args, V8isolate* isolate, i32* result, i32 expected);
bool IsCallingObject(const V8Args& args, V8isolate* isolate, OPscriptTypes type);

const OPchar* ToCString(const v8::String::Utf8Value& value);

#endif

#endif
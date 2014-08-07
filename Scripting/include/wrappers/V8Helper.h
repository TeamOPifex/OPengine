#ifndef OPIFEX_SCRIPTING_WRAPPERS_V8
#define OPIFEX_SCRIPTING_WRAPPERS_V8

#ifdef OPIFEX_V8

#include "v8.h"
using namespace v8;

#include "./Core/include/Types.h"

#ifdef OPIFEX_NODEJS

typedef Arguments V8Args;
typedef Handle<Value> V8Return;
typedef i8* V8isolate;
typedef Handle<ObjectTemplate> V8ObjectT;
typedef Handle<Object> V8Object;
typedef V8Object V8ObjectGlobal;
typedef InvocationCallback V8Function;
typedef HandleScope V8Scope;

extern V8isolate* isolate;

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

inline Handle<Value> GetNumber(V8isolate* isolate, i32 number) {
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

typedef FunctionCallbackInfo<Value> V8Args;
typedef void V8Return;
typedef Isolate V8isolate;
typedef Handle<ObjectTemplate> V8ObjectT;
typedef Handle<Object> V8Object;
typedef V8ObjectT V8ObjectGlobal;
typedef FunctionCallback V8Function;
typedef i8 V8Scope;

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

inline Handle<String> GetString(V8isolate* isolate, i8* name) {
	return String::NewFromUtf8(isolate, name);
}

inline Handle<Value> GetNull(V8isolate* isolate) {
	return Null(isolate);
}

inline Handle<Value> GetNumber(V8isolate* isolate, i32 number) {
	return Number::New(isolate, number);
}

inline Handle<Value> GetBool(V8isolate* isolate, bool val) {
	return Boolean::New(isolate, val);
}

inline Handle<Value> GetNumberF32(V8isolate* isolate, f32 number) {
	return Number::New(isolate, number);
}

inline void SetFunction(V8isolate* isolate, V8Object obj, i8* name, V8Function function) {
	Local<Function> func = Function::New(isolate, function);
	obj->Set(GetString(isolate, name), func);
}

inline void SetFunctionT(V8isolate* isolate, V8ObjectT obj, i8* name, V8Function function) {
	obj->Set(GetString(isolate, name), FunctionTemplate::New(isolate, function));
}

inline void SetFunctionG(V8isolate* isolate, V8ObjectGlobal obj, i8* name, V8Function function) {
	obj->Set(GetString(isolate, name), FunctionTemplate::New(isolate, function));
}

inline void SetValue(V8isolate* isolate, V8Object obj, i8* name, Handle<Value> set) {
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

#endif


void LogProperties(Handle<Object> obj);
void* GetPointer(const V8Args& args, V8isolate* isolate, i32* result, i32 expected);

const char* ToCString(const v8::String::Utf8Value& value);

#endif

#endif
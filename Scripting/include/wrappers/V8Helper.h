#ifndef OPIFEX_SCRIPTING_WRAPPERS_V8
#define OPIFEX_SCRIPTING_WRAPPERS_V8

#ifdef OPIFEX_V8

#include "V8Types.h"
#include "./Scripting/include/OPscriptV8.h"

#ifdef OPIFEX_NODEJS

inline V8Return OPscriptV8SetReturn(const V8Args& args, V8Scope* scope, Handle<Value> val) {
	return scope->Close(val);
}

inline V8Object OPscriptV8CreateObject(V8isolate* isolate) {
	return Object::New();
}

inline V8ObjectGlobal OPscriptV8CreateObjectGlobal(V8isolate* isolate) {
	return Object::New();
}

inline V8ObjectTemplate OPscriptV8CreateObjectTemplate(V8isolate* isolate) {
	return ObjectTemplate::New();
}

inline Handle<String> OPscriptV8GetString(V8isolate* isolate, i8* name) {
	return String::New(name);
}

inline Handle<Value> OPscriptV8GetNull(V8isolate* isolate) {
	return Null();
}

inline Handle<Array> OPscriptV8CreateArray(V8isolate* isolate) {
	return Array::New();
}

inline Handle<Value> OPscriptV8GetNumber(V8isolate* isolate, OPint number) {
	return Number::New(number);
}

inline Handle<Value> OPscriptV8GetBool(V8isolate* isolate, bool val) {
	return Boolean::New(val);
}

inline Handle<Value> OPscriptV8GetNumberF32(V8isolate* isolate, f32 number) {
	return Number::New(number);
}

inline void OPscriptV8SetFunction(V8isolate* isolate, V8Object obj, i8* name, V8Function function) {
	obj->Set(OPscriptV8GetString(isolate, name), FunctionTemplate::New(function)->GetFunction());
}

inline void OPscriptV8SetFunctionTemplate(V8isolate* isolate, V8ObjectTemplate obj, i8* name, V8Function function) {
	obj->Set(OPscriptV8GetString(isolate, name), FunctionTemplate::New(function)->GetFunction());
}

inline void OPscriptV8SetFunctionGlobal(V8isolate* isolate, V8ObjectGlobal obj, i8* name, V8Function function) {
	obj->Set(OPscriptV8GetString(isolate, name), FunctionTemplate::New(function)->GetFunction());
}

inline void OPscriptV8SetValue(V8isolate* isolate, V8Object obj, i8* name, Handle<Value> set) {
	obj->Set(OPscriptV8GetString(isolate, name), set);
}

inline void OPscriptV8SetValueGlobal(V8isolate* isolate, V8ObjectGlobal obj, i8* name, Handle<Value> set) {
	obj->Set(OPscriptV8GetString(isolate, name), set);
}

inline void OPscriptV8SetObject(V8isolate* isolate, V8Object obj, i8* name, V8Object set) {
	obj->Set(OPscriptV8GetString(isolate, name), set);
}

inline void OPscriptV8SetObjectTemplate(V8isolate* isolate, V8ObjectTemplate obj, i8* name, V8ObjectTemplate set) {
	obj->Set(OPscriptV8GetString(isolate, name), set);
}

inline void OPscriptV8SetObjectGlobal(V8isolate* isolate, V8ObjectGlobal obj, i8* name, V8ObjectGlobal set) {
	obj->Set(OPscriptV8GetString(isolate, name), set);
}

#else

inline V8Return OPscriptV8SetReturn(const V8Args& args, V8Scope* scope, Handle<Value> val) {
	return args.GetReturnValue().Set(val);
}

inline V8Object OPscriptV8CreateObject(V8isolate* isolate) {
	return Object::New(isolate);
}

inline V8ObjectTemplate OPscriptV8CreateObjectTemplate(V8isolate* isolate) {
	return ObjectTemplate::New(isolate);
}

inline V8ObjectGlobal OPscriptV8CreateObjectGlobal(V8isolate* isolate) {
	return ObjectTemplate::New(isolate);
}

inline Handle<Array> OPscriptV8CreateArray(V8isolate* isolate) {
	return Array::New(isolate);
}

inline Handle<String> OPscriptV8GetString(V8isolate* isolate, const OPchar* name) {
	return String::NewFromUtf8(isolate, name);
}

inline Handle<Value> OPscriptV8GetNull(V8isolate* isolate) {
	return Null(isolate);
}

inline Handle<Value> OPscriptV8GetNumber(V8isolate* isolate, OPint number) {
	return Number::New(isolate, number);
}

inline Handle<Value> OPscriptV8GetBool(V8isolate* isolate, bool val) {
	return Boolean::New(isolate, val);
}

inline Handle<Value> OPscriptV8GetNumberF32(V8isolate* isolate, f32 number) {
	return Number::New(isolate, number);
}

inline void OPscriptV8SetFunction(V8isolate* isolate, V8Object obj, const OPchar* name, V8Function function) {
	Local<Function> func = Function::New(isolate, function);
	obj->Set(OPscriptV8GetString(isolate, name), func);
}

inline void OPscriptV8SetFunctionTemplate(V8isolate* isolate, V8ObjectTemplate obj, const OPchar* name, V8Function function) {
	obj->Set(OPscriptV8GetString(isolate, name), FunctionTemplate::New(isolate, function));
}

inline void OPscriptV8SetFunctionGlobal(V8isolate* isolate, V8ObjectGlobal obj, const OPchar* name, V8Function function) {
	obj->Set(OPscriptV8GetString(isolate, name), FunctionTemplate::New(isolate, function));
}

inline void OPscriptV8SetValue(V8isolate* isolate, V8Object obj, const OPchar* name, Handle<Value> set) {
	obj->Set(OPscriptV8GetString(isolate, name), set);
}

inline void OPscriptV8SetValueGlobal(V8isolate* isolate, V8ObjectGlobal obj, const OPchar* name, Handle<Value> set) {
	obj->Set(OPscriptV8GetString(isolate, name), set);
}

inline void OPscriptV8SetObject(V8isolate* isolate, V8Object obj, const OPchar* name, V8Object set) {
	obj->Set(OPscriptV8GetString(isolate, name), set);
}

inline void OPscriptV8SetObjectTemplate(V8isolate* isolate, V8ObjectTemplate obj, const OPchar* name, V8ObjectTemplate set) {
	obj->Set(OPscriptV8GetString(isolate, name), set);
}

inline void OPscriptV8SetObjectGlobal(V8isolate* isolate, V8ObjectGlobal obj, const OPchar* name, V8ObjectGlobal set) {
	obj->Set(OPscriptV8GetString(isolate, name), set);
}

inline f32 OPscriptV8GetNumber(V8isolate* isolate, V8Object obj, const OPchar* name, f32 def) {
	Handle<String> n = OPscriptV8GetString(isolate, name);
	if (obj->HasRealNamedProperty(n)) {
		return obj->Get(n)->NumberValue();
	}
	return def;
}

#endif

OPscriptValuePersistent OPscriptGetValue(f32 val); 
OPscriptValuePersistent OPscriptGetValue(const OPchar* val);

void OPscriptV8LogProperties(Handle<Object> obj); 
bool OPscriptV8IsObject(const V8Args& args, V8isolate* isolate, i32 position, OPscriptTypes type);
V8Object OPscriptV8CreateTypedObject(V8isolate* isolate, void* Id, OPscriptTypes type);
void* OPscriptV8GetFirstPointer(const V8Args& args, V8isolate* isolate, i32* result, OPscriptTypes type);
void* OPscriptV8GetCallingPointer(const V8Args& args, V8isolate* isolate);
void* OPscriptV8GetArgPointer(const V8Args& args, V8isolate* isolate, i32 position);
void* OPscriptV8GetPointer(const V8Args& args, V8isolate* isolate, i32* result, i32 expected);
bool OPscriptV8IsCallingObject(const V8Args& args, V8isolate* isolate, OPscriptTypes type);

const OPchar* OPscriptV8ToCString(const v8::String::Utf8Value& value);

#endif

#endif
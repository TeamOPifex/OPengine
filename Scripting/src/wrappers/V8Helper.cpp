#include "../../include/wrappers/V8Helper.h"

#ifdef OPIFEX_V8

#include "./Core/include/Log.h"

#ifdef OPIFEX_NODEJS
V8isolate* isolate = NULL;
#endif

void LogProperties(V8Object obj) {
	Local<Array> arr = obj->GetPropertyNames();
	for (i32 i = 0; i < arr->Length(); i++) {
		Local<String> str = arr->Get(i)->ToString();
		const char *file = *v8::String::Utf8Value(str);
		OPlog(file);
	}

}

V8Object CreateTypedObject(V8isolate* isolate, void* Id, OPscriptTypes type) {

	V8Object obj = CreateObject(isolate);
	obj->Set(GetString(isolate, "Id"), GetNumber(isolate, (i32)Id));
	obj->Set(GetString(isolate, "Type"), GetNumber(isolate, (i32)type));
	return obj;

}

bool IsObject(const V8Args& args, V8isolate* isolate, i32 position, OPscriptTypes type) {
	if (args.Length() > position && args[position]->IsObject()) {
		Handle<Value> result = args[position]->ToObject()->Get(GetString(isolate, "Type"));
		if (!result->IsNull()) {
			return result->Int32Value() == (i32)type;
		}
	}
	return false;
}

bool IsCallingObject(const V8Args& args, V8isolate* isolate, OPscriptTypes type) {
	if (args.This()->IsObject()) {
		if (args.This()->HasOwnProperty(GetString(isolate, "Type"))) {
			return args.This()->Get(GetString(isolate, "Type"))->Int32Value() == (i32)type;
		}
	}
	return false;
}

void* GetCallingPointer(const V8Args& args, V8isolate* isolate) {
		Local<Value> val = args.This()->ToObject()->Get(GetString(isolate, "Id"));
		return (void*)val->Int32Value();
}

void* GetArgPointer(const V8Args& args, V8isolate* isolate, i32 position) {

	if (args.Length() > position) {
		Local<Value> val = args[position]->ToObject()->Get(GetString(isolate, "Id"));
		return (void*)val->Int32Value();
	}

	return NULL;
}

void* GetFirstPointer(const V8Args& args, V8isolate* isolate, i32* result, OPscriptTypes type) {
	if (IsCallingObject(args, isolate, type)) {
		*result = 1;
		return GetCallingPointer(args, isolate);
	}

	if (IsObject(args, isolate, 0, type)) {
		*result = 0;
		return GetArgPointer(args, isolate, 0);
	}

	return NULL;
}

void* GetPointer(const V8Args& args, V8isolate* isolate, i32* result, i32 expected) {

	if (args.Length() < expected) {
		Local<Value> val = args.This()->Get(GetString(isolate, "Id"));
		*result = 1;
		return (void*)val->Int32Value();
	}
	else if (args.Length() >= expected) {
		Local<Value> val = args[0]->ToObject()->Get(GetString(isolate, "Id"));
		*result = 0;
		return (void*)val->Int32Value();
	}

	*result = -1;
	return NULL;
}

const char* ToCString(const v8::String::Utf8Value& value) {
	return *value ? *value : "<string conversion failed>";
}

#endif
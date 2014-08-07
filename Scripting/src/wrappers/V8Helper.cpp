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
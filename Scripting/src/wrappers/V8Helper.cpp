#include "./Scripting/include/wrappers/V8Helper.h"

#ifdef OPIFEX_V8

#include <v8.h>
#include "./Scripting/include/Scripting.h"
#include "./Core/include/Log.h"

void LogProperties(Handle<Object> obj) {
	Local<Array> arr = obj->GetPropertyNames();
	for (i32 i = 0; i < arr->Length(); i++) {
		Local<String> str = arr->Get(i)->ToString();
		const char *file = *v8::String::Utf8Value(str);
		OPlog(file);
	}

}

void* GetPointer(const FunctionCallbackInfo<Value>& args, i32* result, i32 expected) {
	
	if (args.Length() < expected) {
		Local<Value> val = args.This()->Get(String::NewFromUtf8(isolate, "Id"));
		*result = 1;
		return (void*)val->Int32Value();
	}
	else if (args.Length() >= expected) {
		Local<Value> val = args[0]->ToObject()->Get(String::NewFromUtf8(isolate, "Id"));
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
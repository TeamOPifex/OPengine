#include "../../include/wrappers/V8Helper.h"

#ifdef OPIFEX_OPTION_V8

#include "./Core/include/OPlog.h"

#ifdef OPIFEX_OPTION_NODEJS
//V8isolate* isolate = NULL;
#endif

void OPscriptV8LogProperties(V8Object obj) {
	Local<Array> arr = obj->GetPropertyNames();
	for (i32 i = 0; i < arr->Length(); i++) {
		Local<String> str = arr->Get(i)->ToString();
		const char *file = *v8::String::Utf8Value(str);
		OPlog(file);
	}

}

V8Object OPscriptV8CreateTypedObject(V8isolate* isolate, void* Id, OPscriptTypes type) {

	V8Object obj = OPscriptV8CreateObject(isolate);
	obj->Set(OPscriptV8GetString(isolate, "Id"), OPscriptV8GetNumber(isolate, (OPint)Id));
	obj->Set(OPscriptV8GetString(isolate, "Type"), OPscriptV8GetNumber(isolate, (OPint)type));
	return obj;

}

bool OPscriptV8IsObject(const V8Args& args, V8isolate* isolate, i32 position, OPscriptTypes type) {
	if (args.Length() > position && args[position]->IsObject()) {
		Handle<Value> result = args[position]->ToObject()->Get(OPscriptV8GetString(isolate, "Type"));
		if (!result->IsNull()) {
			return result->Int32Value() == (i32)type;
		}
	}
	return false;
}

bool OPscriptV8IsCallingObject(const V8Args& args, V8isolate* isolate, OPscriptTypes type) {
	if (args.This()->IsObject()) {
		if (args.This()->HasOwnProperty(OPscriptV8GetString(isolate, "Type"))) {
			return args.This()->Get(OPscriptV8GetString(isolate, "Type"))->Int32Value() == (i32)type;
		}
	}
	return false;
}

void* OPscriptV8GetCallingPointer(const V8Args& args, V8isolate* isolate) {
		Local<Value> val = args.This()->ToObject()->Get(OPscriptV8GetString(isolate, "Id"));
		return (void*)val->IntegerValue();
}

void* OPscriptV8GetArgPointer(const V8Args& args, V8isolate* isolate, i32 position) {

	if (args.Length() > position) {
		Local<Value> val = args[position]->ToObject()->Get(OPscriptV8GetString(isolate, "Id"));
		return (void*)val->IntegerValue();
	}

	return NULL;
}

void* OPscriptV8GetFirstPointer(const V8Args& args, V8isolate* isolate, i32* result, OPscriptTypes type) {
	if (OPscriptV8IsCallingObject(args, isolate, type)) {
		*result = 1;
		return OPscriptV8GetCallingPointer(args, isolate);
	}

	if (OPscriptV8IsObject(args, isolate, 0, type)) {
		*result = 0;
		return OPscriptV8GetArgPointer(args, isolate, 0);
	}

	return NULL;
}

void* OPscriptV8GetPointer(const V8Args& args, V8isolate* isolate, i32* result, i32 expected) {

	if (args.Length() < expected) {
		Local<Value> val = args.This()->Get(OPscriptV8GetString(isolate, "Id"));
		*result = 1;
		return (void*)val->IntegerValue();
	}
	else if (args.Length() >= expected) {
		Local<Value> val = args[0]->ToObject()->Get(OPscriptV8GetString(isolate, "Id"));
		*result = 0;
		return (void*)val->IntegerValue();
	}

	*result = -1;
	return NULL;
}

const OPchar* OPscriptV8ToCString(const v8::String::Utf8Value& value) {

	return *value ? *value : "<string conversion failed>";
}
#endif


OPscriptValuePersistent OPscriptGetValue(f32 val) {

#ifdef OPIFEX_OPTION_V8
	Isolate::Scope isolate_scope(isolate);
	HandleScope scope(isolate);

	return Persistent<Value>(isolate, Number::New(isolate, val));
#else
	return 0;
#endif
}

OPscriptValuePersistent OPscriptGetValue(const OPchar* val) {
#ifdef OPIFEX_OPTION_V8
	Isolate::Scope isolate_scope(isolate);
	HandleScope scope(isolate);

	return Persistent<Value>(isolate, OPscriptV8GetString(isolate, val));
#else
	return 0;
#endif
}
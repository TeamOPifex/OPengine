#include "./Scripting/include/wrappers/DataWrapper.h"

#ifdef OPIFEX_V8

#include <v8.h>

static void _cmanInit(const FunctionCallbackInfo<Value>& args);
static void _cmanLoad(const FunctionCallbackInfo<Value>& args);
static void _cmanGet(const FunctionCallbackInfo<Value>& args);
static void _cmanDelete(const FunctionCallbackInfo<Value>& args);

void DataInitializeMethods(Isolate* isolate, Handle<ObjectTemplate> target) {

	// OP.cman
	Local<ObjectTemplate> cman = ObjectTemplate::New(isolate);
	cman->Set(String::NewFromUtf8(isolate, "Init"), FunctionTemplate::New(isolate, _cmanInit));
	cman->Set(String::NewFromUtf8(isolate, "Load"), FunctionTemplate::New(isolate, _cmanLoad));
	cman->Set(String::NewFromUtf8(isolate, "Get"), FunctionTemplate::New(isolate, _cmanGet));
	cman->Set(String::NewFromUtf8(isolate, "Delete"), FunctionTemplate::New(isolate, _cmanDelete));
	target->Set(String::NewFromUtf8(isolate, "cman"), cman);
}


#include "./Core/include/Log.h"
#include "./Data/include/ContentManager.h"
#include "./Scripting/include/Scripting.h"
#include "./Scripting/include/wrappers/V8Helper.h"
#include "./Pipeline/include/DefaultLoaders.h"
#include <direct.h>


static void _cmanInit(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() > 0) {
		v8::String::Utf8Value utf8(args[0]);
		const char* v = ToCString(utf8);
		_chdir(v);
	}

	OPcmanInit(OP_DEFAULT_LOADERS, 6);
}

static void _cmanLoad(const FunctionCallbackInfo<Value>& args) {
	v8::String::Utf8Value utf8(args[0]);
	const char* file = ToCString(utf8);
	OPlog("Script Load: %s", file);
	OPint result = OPcmanLoad(file);
	args.GetReturnValue().Set(Integer::New(isolate, result));
}
static void _cmanGet(const FunctionCallbackInfo<Value>& args) {
	v8::String::Utf8Value utf8(args[0]);
	const char* file = ToCString(utf8);
	OPlog("Script Get: %s", file);
	void* p = OPcmanGet(file);
	args.GetReturnValue().Set(Integer::New(isolate, (i32)p));
}
static void _cmanDelete(const FunctionCallbackInfo<Value>& args) {
	v8::String::Utf8Value utf8(args[0]);
	const char* file = ToCString(utf8);
	OPcmanDelete(file);
}

#endif
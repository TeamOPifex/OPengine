#include "../../include/wrappers/DataWrapper.h"

#ifdef OPIFEX_V8

#include "./Core/include/Log.h"
#include "./Data/include/ContentManager.h"
#include "./Pipeline/include/DefaultLoaders.h"

#ifndef OPIFEX_ANDROID
#include <direct.h>
#endif

static V8Return _cmanInit(const V8Args& args);
static V8Return _cmanLoad(const V8Args& args);
static V8Return _cmanGet(const V8Args& args);
static V8Return _cmanDelete(const V8Args& args);

void DataInitializeMethods(V8isolate* isolate, V8ObjectGlobal target) {

	// OP.cman
	V8ObjectGlobal cman = CreateObjectG(isolate);
	SetFunctionG(isolate, cman, "Init", _cmanInit);
	SetFunctionG(isolate, cman, "Load", _cmanLoad);
	SetFunctionG(isolate, cman, "Get", _cmanGet);
	SetFunctionG(isolate, cman, "Delete", _cmanDelete);
	SetObjectG(isolate, target, "cman", cman);

}
void DataInitializeMethodsO(V8isolate* isolate, V8Object target) {

	// OP.cman
	V8Object cman = CreateObject(isolate);
	SetFunction(isolate, cman, "Init", _cmanInit);
	SetFunction(isolate, cman, "Load", _cmanLoad);
	SetFunction(isolate, cman, "Get", _cmanGet);
	SetFunction(isolate, cman, "Delete", _cmanDelete);
	SetObject(isolate, target, "cman", cman);

}


static V8Return _cmanInit(const V8Args& args) {
	V8Scope scope;

#ifndef OPIFEX_ANDROID
	if (args.Length() > 0) {
		v8::String::Utf8Value utf8(args[0]);
		const char* v = ToCString(utf8);
		_chdir(v);
	}
#endif

	OPcmanInit(OP_DEFAULT_LOADERS, 10, NULL);

	return SetReturn(args, &scope, GetNull(isolate));
}

static V8Return _cmanLoad(const V8Args& args) {
	V8Scope scope;

	v8::String::Utf8Value utf8(args[0]);
	const char* file = ToCString(utf8);
	OPlog("Script Load: %s", file);
	OPint result = OPcmanLoad(file);

	return SetReturn(args, &scope, GetNumber(isolate, result));
}

static V8Return _cmanGet(const V8Args& args) {
	V8Scope scope;

	v8::String::Utf8Value utf8(args[0]);
	const char* file = ToCString(utf8);
	OPlog("Script Get: %s", file);
	void* p = OPcmanGet(file);

	return SetReturn(args, &scope, GetNumber(isolate, (i32)p));
}

static V8Return _cmanDelete(const V8Args& args) {
	V8Scope scope;

	v8::String::Utf8Value utf8(args[0]);
	const char* file = ToCString(utf8);
	OPcmanDelete(file);

	return SetReturn(args, &scope, GetNull(isolate));
}

#endif
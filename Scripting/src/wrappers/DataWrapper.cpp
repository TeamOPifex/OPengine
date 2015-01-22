// #include "../../include/wrappers/DataWrapper.h"

// #ifdef OPIFEX_OPTION_V8

// #include "./Core/include/OPlog.h"
// #include "./Data/include/OPcman.h"
// #include "./Pipeline/include/DefaultLoaders.h"

// #ifdef OPIFEX_WINDOWS
// #include <direct.h>
// #endif

// static V8Return _cmanInit(const V8Args& args);
// static V8Return _cmanLoad(const V8Args& args);
// static V8Return _cmanGet(const V8Args& args);
// static V8Return _cmanDelete(const V8Args& args);

// void DataInitializeMethods(V8isolate* isolate, V8ObjectGlobal target) {

// 	// OP.cman
// 	V8ObjectGlobal cman = OPscriptV8CreateObjectGlobal(isolate);
// 	OPscriptV8SetFunctionGlobal(isolate, cman, "Init", _cmanInit);
// 	OPscriptV8SetFunctionGlobal(isolate, cman, "Load", _cmanLoad);
// 	OPscriptV8SetFunctionGlobal(isolate, cman, "Get", _cmanGet);
// 	OPscriptV8SetFunctionGlobal(isolate, cman, "Delete", _cmanDelete);
// 	OPscriptV8SetObjectGlobal(isolate, target, "cman", cman);

// }
// void DataInitializeMethodsO(V8isolate* isolate, V8Object target) {

// 	// OP.cman
// 	V8Object cman = OPscriptV8CreateObject(isolate);
// 	OPscriptV8SetFunction(isolate, cman, "Init", _cmanInit);
// 	OPscriptV8SetFunction(isolate, cman, "Load", _cmanLoad);
// 	OPscriptV8SetFunction(isolate, cman, "Get", _cmanGet);
// 	OPscriptV8SetFunction(isolate, cman, "Delete", _cmanDelete);
// 	OPscriptV8SetObject(isolate, target, "cman", cman);

// }


// static V8Return _cmanInit(const V8Args& args) {
// 	V8Scope scope;

// 	const char* assetDir = NULL;

// #ifndef OPIFEX_ANDROID
// 	if (args.Length() > 0) {
// 		v8::String::Utf8Value utf8(args[0]);
// 		assetDir = OPscriptV8ToCString(utf8);
// 	}
// #endif


// 	OPloadersAddDefault();
// 	OPcmanInit(assetDir);

// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// }

// static V8Return _cmanLoad(const V8Args& args) {
// 	V8Scope scope;

// 	v8::String::Utf8Value utf8(args[0]);
// 	const char* file = OPscriptV8ToCString(utf8);
// 	OPlog("Script Load: %s", file);
// 	OPint result = OPcmanLoad(file);

// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumber(isolate, result));
// }

// static V8Return _cmanGet(const V8Args& args) {
// 	V8Scope scope;

// 	v8::String::Utf8Value utf8(args[0]);
// 	const char* file = OPscriptV8ToCString(utf8);
// 	OPlog("Script Get: %s", file);
// 	void* p = OPcmanGet(file);

// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNumber(isolate, (OPint)p));
// }

// static V8Return _cmanDelete(const V8Args& args) {
// 	V8Scope scope;

// 	v8::String::Utf8Value utf8(args[0]);
// 	const char* file = OPscriptV8ToCString(utf8);
// 	OPcmanDelete(file);

// 	return OPscriptV8SetReturn(args, &scope, OPscriptV8GetNull(isolate));
// }

// #endif
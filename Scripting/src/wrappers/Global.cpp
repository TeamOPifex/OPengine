#include "../../include/wrappers/Global.h"

#ifdef OPIFEX_V8

#include "./Core/include/Core.h"
#include "./Core/include/Log.h"
#include "./Core/include/Timer.h"

static V8Return _start(const V8Args& args);

void GlobalInitializeMethods(V8isolate* isolate, V8ObjectGlobal target) {

	SetFunctionG(isolate, target, "Start", _start);

}

Local<Function> InitializeCallback;
Local<Function> UpdateCallback;
Local<Function> DestroyCallback;

void ScriptingInit(){
	const unsigned argc = 1;
	Handle<Value> argv[argc] = { GetBool(isolate, true) };
	Handle<Object> obj = CreateObject(isolate);
	InitializeCallback->Call(obj, argc, argv);
}

int ScriptingUpdate(OPtimer* timer){
	const unsigned argc = 1;
	Handle<Value> argv[argc] = { GetNumberF32(isolate, timer->Elapsed) };
	Handle<Object> obj = CreateObject(isolate);
	Local<Value> result = UpdateCallback->Call(obj, argc, argv);
	if (result->IsNumber())
		return result->Int32Value();

	return 0;
}

void ScriptingDestroy()
{
	const unsigned argc = 1;
	Handle<Value> argv[argc] = { GetBool(isolate, true) };
	Handle<Object> obj = CreateObject(isolate);
	DestroyCallback->Call(obj, argc, argv);
	return;
}

static V8Return _start(const V8Args& args) {
	V8Scope scope;

	InitializeCallback = Local<Function>::Cast(args[0]);
	UpdateCallback = Local<Function>::Cast(args[1]);
	DestroyCallback = Local<Function>::Cast(args[2]);

	OPinitialize = ScriptingInit;
	OPupdate = ScriptingUpdate;
	OPdestroy = ScriptingDestroy;

	OPlog("Engine Initialized");

	OPstart();
	OPend();

	return SetReturn(args, &scope, GetNull(isolate));
}

#endif
#include "./Scripting/include/wrappers/Global.h"

#ifdef OPIFEX_V8

#include <v8.h>

static void _start(const FunctionCallbackInfo<Value>& args);

void GlobalInitializeMethods(Isolate* isolate, Handle<ObjectTemplate> target) {

	target->Set(String::NewFromUtf8(isolate, "Start"), FunctionTemplate::New(isolate, _start));

}

#include "./Core/include/Core.h"
#include "./Core/include/Log.h"
#include "./Core/include/Timer.h"
#include "./Scripting/include/Scripting.h"

Local<Function> InitializeCallback;
Local<Function> UpdateCallback;
Local<Function> DestroyCallback;

void ScriptingInit(){
	const unsigned argc = 1;
	Local<Value> argv[argc] = { Boolean::New(isolate, true) };

	InitializeCallback->Call(Boolean::New(isolate, true), argc, argv);
}

int ScriptingUpdate(OPtimer* timer){
	const unsigned argc = 1;
	Local<Value> argv[argc] = { Number::New(isolate, timer->Elapsed) };
	Local<Value> result = UpdateCallback->Call(Boolean::New(isolate, true), argc, argv);
	if (result->IsNumber())
		return result->Int32Value();

	return 0;
}

void ScriptingDestroy()
{
	const unsigned argc = 1;
	Local<Value> argv[argc] = { Boolean::New(isolate, true) };
	DestroyCallback->Call(Boolean::New(isolate, true), argc, argv);
	return;
}

static void _start(const FunctionCallbackInfo<Value>& args) {
	v8::HandleScope scope(args.GetIsolate());

	InitializeCallback = Local<Function>::Cast(args[0]);
	UpdateCallback = Local<Function>::Cast(args[1]);
	DestroyCallback = Local<Function>::Cast(args[2]);

	OPinitialize = ScriptingInit;
	OPupdate = ScriptingUpdate;
	OPdestroy = ScriptingDestroy;

	OPlog("Engine Initialized");

	OPstart();
	OPend();
}

#endif
#include "./Core/include/Log.h"
#include "./Scripting/include/Scripting.h"

#ifdef OPIFEX_V8

static void LogCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
	if (args.Length() < 1) return;
	HandleScope scope(args.GetIsolate());
	Handle<Value> arg = args[0];
	String::Utf8Value value(arg);
	OPlog("Logged: %s", *value);
}

Isolate* isolate = NULL;
#endif

void OPscriptInit() {
#ifdef OPIFEX_V8
	v8::V8::InitializeICU();
	v8::Platform* platform = v8::platform::CreateDefaultPlatform();
	v8::V8::InitializePlatform(platform);
	isolate = v8::Isolate::New();
#else
	OPlog("V8 Engine Feature not enabled.");
#endif
}

#ifdef OPIFEX_V8
void Print(const v8::FunctionCallbackInfo<v8::Value>& args) {
	for (int i = 0; i < args.Length(); i++) {
		if (i != 0) printf(" ");
		v8::HandleScope scope(args.GetIsolate());
		v8::String::Utf8Value str(args[i]);
		if (*str == NULL) return;
		printf("%s", *str);
	}
	printf("\n");
}
#endif

#include "./Scripting/include/wrappers/HumanWrapper.h"
#include "./Scripting/include/wrappers/DataWrapper.h"
#include "./Scripting/include/wrappers/MathWrapper.h"
#include "./Scripting/include/wrappers/Global.h"

void OPscriptCompileAndRun(OPscript* script) {
#ifdef OPIFEX_V8
	Isolate::Scope isolate_scope(isolate);
	HandleScope scope(isolate);

	Handle<ObjectTemplate> global = ObjectTemplate::New(isolate);
	Handle<ObjectTemplate> OP = ObjectTemplate::New(isolate);
	GlobalInitializeMethods(isolate, OP);
	HumanInitializeMethods(isolate, OP);
	MathInitializeMethods(isolate, OP);
	DataInitializeMethods(isolate, OP);
	global->Set(String::NewFromUtf8(isolate, "print"), FunctionTemplate::New(isolate, Print));
	global->Set(String::NewFromUtf8(isolate, "OP"), OP);
	Local<Context> context = Context::New(isolate, NULL, global);
	v8::Context::Scope context_scope(context);

	Handle<String> source = String::NewFromUtf8(isolate, script->data);

	v8::ScriptOrigin origin(v8::String::NewFromUtf8(context->GetIsolate(), "name"));
	v8::Handle<v8::Script> compiled = v8::Script::Compile(source, &origin);
	compiled->Run();
#endif
}

void OPscriptCompileAndRunStream(OPstream* stream) {
#ifdef OPIFEX_V8
	Isolate::Scope isolate_scope(isolate);
	HandleScope scope(isolate);

	Handle<ObjectTemplate> global = ObjectTemplate::New(isolate);
	Handle<ObjectTemplate> OP = ObjectTemplate::New(isolate);
	GlobalInitializeMethods(isolate, OP);
	HumanInitializeMethods(isolate, OP);
	MathInitializeMethods(isolate, OP);
	DataInitializeMethods(isolate, OP);

	global->Set(String::NewFromUtf8(isolate, "print"), FunctionTemplate::New(isolate, Print));
	global->Set(String::NewFromUtf8(isolate, "OP"), OP);
	Local<Context> context = Context::New(isolate, NULL, global);
	v8::Context::Scope context_scope(context);

	Handle<String> source = String::NewFromUtf8(isolate, (i8*)stream->Data);

	v8::ScriptOrigin origin(v8::String::NewFromUtf8(context->GetIsolate(), "name"));
	v8::Handle<v8::Script> compiled = v8::Script::Compile(source, &origin);
	compiled->Run();
#endif
}

void OPscriptLog(const char* data) {
#ifdef OPIFEX_V8
	//HandleScope scope(isolate);
	//Local<Context> local_context = Context::New(isolate, context);
	//Context::Scope context_scope(local_context);

	//Local<Object> global = local_context->Global();
	//Local<Value> value = global->Get(String::NewFromUtf8(isolate, "log"));

	//if (value->IsFunction()) {
	//	Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(value);
	//	Handle<Value> args[1];
	//	args[0] = v8::String::NewFromUtf8(isolate, data);
	//	func->Call(global, 1, args);
	//}
#endif
}

void OPscriptRun(Handle<Script> script) {
#ifdef OPIFEX_V8
	HandleScope scope(isolate);
	v8::Handle<v8::Value> result = script->Run();


	//Handle<v8::Object> global = context->Global();
	//Handle<Value> val = String::NewFromUtf8(isolate, func);
	//Handle<v8::Value> value = global->Get(val);

	//if (value->IsFunction()) {
	//	Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(value);		
	//	Handle<Value> args[1];
	//	args[0] = v8::String::NewFromUtf8(isolate, "test");
	//	func->Call(global, 1, args);
	//}
#endif
}

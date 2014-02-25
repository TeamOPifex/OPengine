#include "./Core/include/Log.h"
#include "./Scripting/include/Scripting.h"

#ifdef OPIFEX_V8
#include "v8.h"
using namespace v8;

static void LogCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
	if (args.Length() < 1) return;
	HandleScope scope(args.GetIsolate());
	Handle<Value> arg = args[0];
	String::Utf8Value value(arg);
	OPlog("Logged: %s", *value);
}

Isolate* isolate;
Persistent<Context> context;
#endif

void OPscriptInit() {
#ifdef OPIFEX_V8
	isolate = Isolate::GetCurrent();
	
	// Create a stack-allocated handle scope.
	HandleScope handle_scope(isolate);

	Handle<ObjectTemplate> global = ObjectTemplate::New();
	global->Set(String::NewFromUtf8(isolate, "log"), FunctionTemplate::New(isolate, LogCallback));

	// Create a new context.
	Handle<Context> local_context = Context::New(isolate, NULL, global);
	context.Reset(isolate, local_context);
#endif
}

void OPscriptCompile(OPscript* script) {
#ifdef OPIFEX_V8
	HandleScope scope(isolate);
	Local<Context> local_context = Local<Context>::New(isolate, context);
	Context::Scope context_scope(local_context);

	// Create a string containing the JavaScript source code.
	Handle<String> source = String::NewFromUtf8(isolate, script->data);

	// Compile the source code.
	Handle<Script> compiled = Script::Compile(source);

	// Run the script to get the result.
	Handle<Value> result = compiled->Run();
#endif
}

void OPscriptLog(const char* data) {
#ifdef OPIFEX_V8
	HandleScope scope(isolate);
	Local<Context> local_context = Local<Context>::New(isolate, context);
	Context::Scope context_scope(local_context);

	Handle<v8::Object> global = local_context->Global();
	Handle<Value> val = String::NewFromUtf8(isolate, "log");
	Handle<v8::Value> value = global->Get(val);

	if (value->IsFunction()) {
		Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(value);
		Handle<Value> args[1];
		args[0] = v8::String::NewFromUtf8(isolate, data);
		func->Call(global, 1, args);
	}
#endif
}

void OPscriptRun(const char* func) {
#ifdef OPIFEX_V8
	HandleScope scope(isolate);
	Local<Context> local_context = Local<Context>::New(isolate, context);
	Context::Scope context_scope(local_context);

	Handle<v8::Object> global = local_context->Global();
	Handle<Value> val = String::NewFromUtf8(isolate, func);
	Handle<v8::Value> value = global->Get(val);

	if (value->IsFunction()) {
		Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(value);		
		Handle<Value> args[1];
		args[0] = v8::String::NewFromUtf8(isolate, "test");
		func->Call(global, 1, args);
	}
#endif
}

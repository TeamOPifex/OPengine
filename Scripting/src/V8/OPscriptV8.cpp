#include "./Scripting/include/V8/OPscriptV8.h"

#ifdef OPIFEX_OPTION_V8

#include "./include/libplatform/libplatform.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPmemory.h"
#include "./Data/include/OPstring.h"
#include "./Data/include/OPcman.h"
#include "./Core/include/Assert.h"

#include "./Scripting/include/V8/OPscriptV8WrapperMath.h"
#include "./Scripting/include/V8/OPscriptV8WrapperHuman.h"

OPscriptV8Isolate* OPSCRIPTV8_ISOLATE = NULL;
void(*OPSCRIPTV8_WRAPPER)(Handle<ObjectTemplate>) = NULL;
OPint(*OPSCRIPTV8_REQUIRE)(FunctionCallbackInfo<Value>) = NULL;

void OPscriptV8Init() {
	if(OPSCRIPTV8_ISOLATE == NULL) {
		v8::V8::InitializeICU();
		v8::Platform* platform = v8::platform::CreateDefaultPlatform();
		v8::V8::InitializePlatform(platform);
		v8::V8::Initialize();
		OPSCRIPTV8_ISOLATE = Isolate::New();
	}
}

void _OPscriptV8Log(const v8::FunctionCallbackInfo<v8::Value>& args) {
	for (int i = 0; i < args.Length(); i++) {
		v8::HandleScope scope(args.GetIsolate());
		v8::String::Utf8Value str(args[i]);
		if (*str == NULL) return;
		OPlog("%s", *str);
	}
}

void _OPscriptV8SetConsole(Handle<ObjectTemplate> objTemplate) {
	Handle<ObjectTemplate> console = ObjectTemplate::New(OPSCRIPTV8_ISOLATE);

	console->Set(
		String::NewFromUtf8(OPSCRIPTV8_ISOLATE, "log"), 
		FunctionTemplate::New(OPSCRIPTV8_ISOLATE, _OPscriptV8Log));

	objTemplate->Set(
		String::NewFromUtf8(OPSCRIPTV8_ISOLATE, "console"), console);
}

void _OPscriptV8WrapEngine(const v8::FunctionCallbackInfo<v8::Value>& args) {
	Handle<Object> OP = Object::New(OPSCRIPTV8_ISOLATE);
	OPscriptV8WrapperHuman(OP);
	OPscriptV8WrapperMath(OP);
	// GlobalInitializeMethodsO(OPSCRIPTV8_ISOLATE, OP);
	// HumanInitializeMethodsO(OPSCRIPTV8_ISOLATE, OP);
	// MathInitializeMethodsO(OPSCRIPTV8_ISOLATE, OP);
	// PerformanceInitializeMethodsO(OPSCRIPTV8_ISOLATE, OP);
	// DataInitializeMethodsO(OPSCRIPTV8_ISOLATE, OP);
	args.GetReturnValue().Set(OP);
}

OPchar* _OPscriptV8NormalizePath(const OPchar* path) {

	// Get a copy of the current argument path that we can use to alter it
	OPchar* pathCopy = OPstringCopy(path);
	OPchar* pathToLoad = pathCopy;
	OPlogDebug("OPscriptV8Require: %s", pathToLoad);

	const OPchar* begin = "./";
	const OPchar* jsEnd = ".js";
	const OPchar* opsEnd = ".ops";

	// If the require starts with a ./ then remove it
	if(OPstringStartsWith(pathToLoad, "./")) {
		pathToLoad = &pathToLoad[2];
	}

	// If the require doesn't end with a .js or a .ops then add a .js to the end
	if (!OPstringEndsWith(pathToLoad, jsEnd) && !OPstringEndsWith(pathToLoad, opsEnd)) {
		OPchar* res = OPstringCreateMerged(pathToLoad, jsEnd);
		OPfree(pathToLoad);
		pathToLoad = res;
	}

	return pathToLoad;
}

void _OPscriptV8Require(const v8::FunctionCallbackInfo<v8::Value>& args) {

	if (args.Length() == 0 || !args[0]->IsString()) return;

	v8::String::Utf8Value utf8(args[0]);
	const char* arg0 = *utf8;

	if (OPstringEquals(arg0, "OPengine")) {
		_OPscriptV8WrapEngine(args);
		return;
	}

	if (OPSCRIPTV8_REQUIRE == NULL || !OPSCRIPTV8_REQUIRE(args)) {

		OPchar* pathToLoad = _OPscriptV8NormalizePath(arg0);
		
		if(OPcmanLoad(pathToLoad) == 0) return;

		OPscript* scriptSource = (OPscript*)OPcmanGet(pathToLoad);

		OPscriptV8Compiled compiled;
		OPscriptV8Compile(&compiled, scriptSource, NULL);


		Local<Value> result = OPscriptV8Run(&compiled);
		Handle<Context> localContext = Local<Context>::New(OPSCRIPTV8_ISOLATE, compiled.Context);
		Local<Value> exports = localContext->Global()->Get(
			String::NewFromUtf8(OPSCRIPTV8_ISOLATE, "module"))->ToObject()->Get(
				String::NewFromUtf8(OPSCRIPTV8_ISOLATE, "exports"));

		args.GetReturnValue().Set(exports);

		OPfree(pathToLoad);

	}
}

OPint OPscriptV8Compile(OPscriptV8Compiled* compiled, OPscript* script, OPscriptV8PersistentContext* context) {
	ASSERT(OPSCRIPTV8_ISOLATE != NULL, "V8 Engine must be initialized first.");

	Isolate::Scope isolate_scope(OPSCRIPTV8_ISOLATE);
	HandleScope scope(OPSCRIPTV8_ISOLATE);

	Handle<ObjectTemplate> global = ObjectTemplate::New(OPSCRIPTV8_ISOLATE);		
	global->Set(String::NewFromUtf8(OPSCRIPTV8_ISOLATE, "module"), ObjectTemplate::New(OPSCRIPTV8_ISOLATE));
	global->Set(String::NewFromUtf8(OPSCRIPTV8_ISOLATE, "global"), ObjectTemplate::New(OPSCRIPTV8_ISOLATE));
	global->Set(String::NewFromUtf8(OPSCRIPTV8_ISOLATE, "require"), FunctionTemplate::New(OPSCRIPTV8_ISOLATE, _OPscriptV8Require));
	_OPscriptV8SetConsole(global);

	if (OPSCRIPTV8_WRAPPER != NULL) {
		OPSCRIPTV8_WRAPPER(global);
	}

	Handle<Context> localContext;
	if(context == NULL) {
		localContext = Context::New(OPSCRIPTV8_ISOLATE, NULL, global);
	} else {
		localContext = Local<Context>::New(OPSCRIPTV8_ISOLATE, *context);
	}

	v8::Context::Scope context_scope(localContext);

	Handle<String> source = String::NewFromUtf8(OPSCRIPTV8_ISOLATE, script->data);

	v8::ScriptOrigin origin(v8::String::NewFromUtf8(localContext->GetIsolate(), "name"));
	Handle<Script> compiledV8 = v8::Script::Compile(source, &origin);
	if (compiledV8.IsEmpty()) {
		return 0;
	}

	OPscriptV8Compiled result = { 
		script,
		Persistent<Script, CopyablePersistentTraits<Script> >(OPSCRIPTV8_ISOLATE, compiledV8),
		Persistent<Context, CopyablePersistentTraits<Context> >(OPSCRIPTV8_ISOLATE, localContext),
	};

	*compiled = result;
	return 1;
}

OPint OPscriptV8Compile(OPscriptV8Compiled* compiled, OPscript* script) {
	OPscriptV8Compile(compiled, script, NULL);
}

Local<Value> _runCompiled(OPscriptV8Compiled* scriptCompiled) {

	Isolate::Scope isolate_scope(OPSCRIPTV8_ISOLATE);
	HandleScope scope(OPSCRIPTV8_ISOLATE);

	Handle<Context> context = Local<Context>::New(OPSCRIPTV8_ISOLATE, scriptCompiled->Context);
	v8::Context::Scope context_scope(context);

	Handle<Script> compiled = Local<Script>::New(OPSCRIPTV8_ISOLATE, scriptCompiled->Script);

	TryCatch trycatch;
	Local<Value> result = compiled->Run();
	if (result.IsEmpty()) {
		Local<Value> exception = trycatch.Exception();
		String::Utf8Value exception_str(exception);
		OPlog("OPscriptV8: Exception: %s", *exception_str);
	}
	return result;
}

void OPscriptV8Update(OPscriptV8Compiled* scriptCompiled) {
	if (scriptCompiled->Source->changed) {
		OPscriptV8Compiled temp;
		if (OPscriptV8Compile(&temp, scriptCompiled->Source, &scriptCompiled->Context)) {
			*scriptCompiled = temp;
			_runCompiled(scriptCompiled);
		}
		scriptCompiled->Source->changed = 0;
	}
}

Local<Value> OPscriptV8Run(OPscriptV8Compiled* scriptCompiled) {
	OPSCRIPTV8_UPDATE(scriptCompiled);
	return _runCompiled(scriptCompiled);	
}

OPscriptV8PersistentValue OPscriptV8Run(OPscriptV8Compiled* scriptCompiled, OPchar* name, OPint count, OPscriptV8PersistentValue* args) {
	OPSCRIPTV8_UPDATE(scriptCompiled);

	Isolate::Scope isolate_scope(OPSCRIPTV8_ISOLATE);
	HandleScope scope(OPSCRIPTV8_ISOLATE);

	Handle<Context> context = Local<Context>::New(OPSCRIPTV8_ISOLATE, scriptCompiled->Context);
	v8::Context::Scope context_scope(context);
	Handle<v8::Object> global = context->Global();
	Handle<v8::Value> value = global->Get(String::NewFromUtf8(OPSCRIPTV8_ISOLATE, name));
	Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(value);

	Handle<Value> values[10];
	for (OPint i = 0; i < count; i++) {
		values[i] = Local<Value>::New(OPSCRIPTV8_ISOLATE, args[i]);
	}
	
	return Persistent<Value>(OPSCRIPTV8_ISOLATE, func->Call(global, count, values));
}

OPscriptV8PersistentValue OPscriptV8Run(OPscriptV8Compiled* scriptCompiled, OPchar* name) {
	return OPscriptV8Run(scriptCompiled, name, 0, NULL);
}

OPscriptV8PersistentObject OPscriptV8CreateObject() {
	Isolate::Scope isolate_scope(OPSCRIPTV8_ISOLATE);
	HandleScope scope(OPSCRIPTV8_ISOLATE);

	return Persistent<Object>(OPSCRIPTV8_ISOLATE, Object::New(OPSCRIPTV8_ISOLATE));
}

#endif
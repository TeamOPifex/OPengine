#include "./Core/include/OPlog.h"
#include "./Scripting/include/OPscript.h"

#ifdef OPIFEX_V8
void Print(const v8::FunctionCallbackInfo<v8::Value>& args);
void Require(const v8::FunctionCallbackInfo<v8::Value>& args);
void(*CustomWrapper)(V8isolate* isolate, V8ObjectGlobal target) = NULL;

static void LogCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
	if (args.Length() < 1) return;
	HandleScope scope(args.GetIsolate());
	Handle<Value> arg = args[0];
	String::Utf8Value value(arg);
	OPlog("Logged: %s", *value);
}

#endif


#ifdef OPIFEX_V8
#include "./Scripting/include/wrappers/HumanWrapper.h"
#include "./Scripting/include/wrappers/DataWrapper.h"
#include "./Scripting/include/wrappers/PerformanceWrapper.h"
#include "./Scripting/include/wrappers/MathWrapper.h"
#include "./Scripting/include/wrappers/Global.h"
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
#include "./Data/include/OPcman.h"

bool StartsWith(OPchar* str, const OPchar* cmp, i32 size) {
	ui32 lenA = strlen(str);
	ui32 lenB = strlen(cmp);
	if (lenA < size || lenB < size) return false;
	return OPmemcmp(str, cmp, size) == 0;
}

void RemoveFromStart(OPchar* str, i32 size) {
	i32 len = strlen(str);
	for (i32 i = 0; i < len - size; i++) {
		str[i] = str[i + size];
	}
	str[len - size] = '\0';
}

OPchar* GetNonConstant(const OPchar* str) {
	i32 len = strlen(str);
	OPchar* result = (OPchar*)OPalloc(sizeof(OPchar)* (len + 1));
	strcpy(result, str);
	return result;
}

OPchar* AddToString(OPchar* str, const OPchar* add) {
	i32 lenA = strlen(str);
	i32 lenB = strlen(add);
	OPchar* result = (OPchar*)OPalloc(lenA + lenB + 1);
	strcpy(result, str);
	strcat(result, add);
	return result;
}

OPchar* PrependToString(OPchar* str, const OPchar* add) {
	i32 lenA = strlen(str);
	i32 lenB = strlen(add);
	OPchar* result = (OPchar*)OPalloc(sizeof(OPchar)* (lenA + lenB + 1));
	strcpy(result, add);
	strcat(result, str);
	return result;
}

#include "./Data/include/OPfile.h"

void Require(const v8::FunctionCallbackInfo<v8::Value>& args) {

	if (args.Length() > 0 && args[0]->IsString()) {

		v8::String::Utf8Value utf8(args[0]);
		const char* p = OPscriptV8ToCString(utf8);
		ui32 len = strlen(p);

		const OPchar* OPengineNode = "./node_modules/OPengine/OPengine";
		const OPchar* OPengine = "OPengine";
		if ((strlen(OPengine) == len && OPmemcmp(OPengine, p, len) == 0) || (strlen(OPengineNode) == len && OPmemcmp(OPengineNode, p, len) == 0)) {

			Handle<Object> OP = Object::New(isolate);
			GlobalInitializeMethodsO(isolate, OP);
			HumanInitializeMethodsO(isolate, OP);
			MathInitializeMethodsO(isolate, OP);
			PerformanceInitializeMethodsO(isolate, OP);
			DataInitializeMethodsO(isolate, OP);
			args.GetReturnValue().Set(OP);
			//OPlog("Loaded OP");
			return;
		}
		else {
			OPchar* tmp = GetNonConstant(p);
			OPchar* loadFile = tmp;
			OPlog("Require: %s", loadFile);

			const OPchar* begin = "./";
			const OPchar* jsEnd = ".js";
			const OPchar* opsEnd = ".ops";
			const OPchar* dir = "assets/Scripts/";

			if (StartsWith(loadFile, begin, 2)) {
				OPlog("Starts with %s, removing it", begin);
				RemoveFromStart(loadFile, 2);
			}

			if (!StartsWith(loadFile, jsEnd, 3) || !StartsWith(loadFile, opsEnd, 4)) {
				OPlog("Doesn't start with %s or %s, adding .js", jsEnd, opsEnd);
				OPchar* res = AddToString(loadFile, jsEnd);
				OPlog("Result %s", res);
				OPfree(loadFile);
				OPlog("Old array freed");
				loadFile = res;
			}

			OPlog("Prepending String");

			loadFile = PrependToString(loadFile, dir);

			OPlog("Require: %s", loadFile);

			OPstream* stream = OPreadFile(loadFile);
			Handle<String> source = String::NewFromUtf8(isolate, (OPchar*)stream->Data);


			Handle<ObjectTemplate> global = ObjectTemplate::New(isolate);
			Handle<ObjectTemplate> empty = ObjectTemplate::New(isolate);
			Handle<ObjectTemplate> module = ObjectTemplate::New(isolate);
			global->Set(String::NewFromUtf8(isolate, "module"), module);
			global->Set(String::NewFromUtf8(isolate, "require"), FunctionTemplate::New(isolate, Require));
			global->Set(String::NewFromUtf8(isolate, "global"), empty);
			Handle<ObjectTemplate> console = ObjectTemplate::New(isolate);
			console->Set(String::NewFromUtf8(isolate, "log"), FunctionTemplate::New(isolate, Print));
			global->Set(String::NewFromUtf8(isolate, "console"), console);

			if (CustomWrapper != NULL) {
				CustomWrapper(isolate, global);
			}
			Local<Context> context = Context::New(isolate, NULL, global);
			v8::Context::Scope context_scope(context);

			v8::ScriptOrigin origin(v8::String::NewFromUtf8(context->GetIsolate(), "name"));
			v8::Handle<v8::Script> compiled = v8::Script::Compile(source, &origin);
			if (compiled.IsEmpty()) {
				OPlog("FAILED to Compile: %s", loadFile);
				return;
			}
			OPlog("Compiled: %s", loadFile);
			TryCatch trycatch;
			Local<Value> result = compiled->Run();
			if (result.IsEmpty()) {
				Local<Value> exception = trycatch.Exception();
				String::Utf8Value exception_str(exception);
				OPlog("Exception: %s", *exception_str);
			}
			OPlog("Script ran");

			OPlog("Processed: %s", loadFile);
			Local<Value> exports = context->Global()->Get(String::NewFromUtf8(isolate, "module"))->ToObject()->Get(String::NewFromUtf8(isolate, "exports"));

			args.GetReturnValue().Set(exports);

			OPfree(loadFile);


		}
	}
}
#endif

OPint OPscriptCompile(OPscriptCompiled* scriptCompiled, OPscript* script, OPscriptPersistentContext* existingContext) {
	OPscriptInit();
#ifdef OPIFEX_V8
	Isolate::Scope isolate_scope(isolate);
	HandleScope scope(isolate);

	Handle<ObjectTemplate> global = ObjectTemplate::New(isolate);
	if (CustomWrapper != NULL) {
		CustomWrapper(isolate, global);
	}
	global->Set(String::NewFromUtf8(isolate, "require"), FunctionTemplate::New(isolate, Require));
	global->Set(String::NewFromUtf8(isolate, "print"), FunctionTemplate::New(isolate, Print));
	global->Set(String::NewFromUtf8(isolate, "global"), ObjectTemplate::New(isolate));

	Handle<Context> context;
	if (existingContext == NULL) {
		context = Context::New(isolate, NULL, global);
	}
	else {
		context = Local<Context>::New(isolate, *existingContext);
	}
	v8::Context::Scope context_scope(context);

	Handle<String> source = String::NewFromUtf8(isolate, script->data);

	v8::ScriptOrigin origin(v8::String::NewFromUtf8(context->GetIsolate(), "name"));
	Handle<Script> compiled = v8::Script::Compile(source, &origin);
	if (compiled.IsEmpty()) {
		return 0;
	}

	OPscriptCompiled result = { 
		script,
		Persistent<Script, CopyablePersistentTraits<Script> >(isolate, compiled),
		Persistent<Context, CopyablePersistentTraits<Context> >(isolate, context),
		Persistent<ObjectTemplate, CopyablePersistentTraits<ObjectTemplate> >(isolate, global)
	};

	*scriptCompiled = result;
	return 1;
#endif
	return 0;
}

#ifdef OPIFEX_V8
void _runCompiled(OPscriptCompiled* scriptCompiled) {

	Isolate::Scope isolate_scope(isolate);
	HandleScope scope(isolate);

	Handle<Context> context = Local<Context>::New(isolate, scriptCompiled->context);
	v8::Context::Scope context_scope(context);

	Handle<Script> compiled = Local<Script>::New(isolate, scriptCompiled->result);

	Local<Value> result = compiled->Run();
}
#endif

void OPscriptUpdate(OPscriptCompiled* scriptCompiled) {
#if defined(_DEBUG) && defined(OPIFEX_V8)
	if (scriptCompiled->source->changed) {
		OPscriptCompiled temp;
		if (OPscriptCompile(&temp, scriptCompiled->source, &scriptCompiled->context)) {
			*scriptCompiled = temp;
			_runCompiled(scriptCompiled);
		}
		scriptCompiled->source->changed = 0;
	}
#endif
}

OPscriptValuePersistent OPscriptRunFunc(OPscriptCompiled* scriptCompiled, OPchar* name, OPint count, OPscriptValuePersistent* args) {
#ifdef OPIFEX_V8
	OPscriptUpdate(scriptCompiled);
	
	Isolate::Scope isolate_scope(isolate);
	HandleScope scope(isolate);

	Handle<Context> context = Local<Context>::New(isolate, scriptCompiled->context);
	v8::Context::Scope context_scope(context);
	Handle<v8::Object> global = context->Global();
	Handle<v8::Value> value = global->Get(String::NewFromUtf8(isolate, name));
	Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(value);

	Handle<Value> values[10];
	for (OPint i = 0; i < count; i++) {
		values[i] = Local<Value>::New(isolate, args[i]);
	}
	
	return Persistent<Value>(isolate, func->Call(global, count, values));
#else
	return 0;
#endif
}

void OPscriptRun(OPscriptCompiled* scriptCompiled) {
#ifdef OPIFEX_V8
	OPscriptUpdate(scriptCompiled);
	_runCompiled(scriptCompiled);	
#endif
}

void OPscriptCompileAndRun(OPscript* script) {
#ifdef OPIFEX_V8
	OPscriptCompiled result;
	if (OPscriptCompile(&result, script, NULL)) {
		OPscriptRun(&result);
	}
#endif
}

void OPscriptCompileAndRunStream(OPstream* stream) {
#ifdef OPIFEX_V8
	Isolate::Scope isolate_scope(isolate);
	HandleScope scope(isolate);

	Handle<ObjectTemplate> global = ObjectTemplate::New(isolate);
	Handle<ObjectTemplate> empty = ObjectTemplate::New(isolate);
	Handle<ObjectTemplate> console = ObjectTemplate::New(isolate);
	console->Set(String::NewFromUtf8(isolate, "log"), FunctionTemplate::New(isolate, Print));
	if (CustomWrapper != NULL) {
		CustomWrapper(isolate, global);
	}

	global->Set(String::NewFromUtf8(isolate, "require"), FunctionTemplate::New(isolate, Require));
	global->Set(String::NewFromUtf8(isolate, "print"), FunctionTemplate::New(isolate, Print));
	global->Set(String::NewFromUtf8(isolate, "global"), empty);
	global->Set(String::NewFromUtf8(isolate, "console"), console);

	Local<Context> context = Context::New(isolate, NULL, global);
	v8::Context::Scope context_scope(context);

	Handle<String> source = String::NewFromUtf8(isolate, (OPchar*)stream->Data);

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

#ifdef OPIFEX_V8
void OPscriptRun(Handle<Script> script) {
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
}
#endif

#include "./Scripting/include/JavaScript/OPjavaScriptV8.h"
#include "./Scripting/include/JavaScript/OPjavaScriptHelper.h"

#ifdef OPIFEX_OPTION_V8

#include "./Core/include/Assert.h"
#include "./include/libplatform/libplatform.h"
#include "./Data/include/OPstring.h"
#include "./Data/include/OPcman.h"
#include "./v8-debug.h"

#define CACHED_FILE_COUNT 256

Isolate* isolate = NULL;
OPint(*OPJAVASCRIPTV8_REQUIRE)(FunctionCallbackInfo<Value>) = NULL;
void(*OPJAVASCRIPTV8_CUSTOMWRAPPER)(Handle<Object>) = NULL;

class ArrayBufferAllocator : public v8::ArrayBuffer::Allocator {
public:
	virtual void* Allocate(size_t length) {
		void* data = AllocateUninitialized(length);
		return data == NULL ? data : memset(data, 0, length);
	}
	virtual void* AllocateUninitialized(size_t length) { return malloc(length); }
	virtual void Free(void* data, size_t) { free(data); }
};

ArrayBufferAllocator allocator;

void OPjavaScriptV8Init() {
	if(isolate == NULL) {
		if (v8::V8::InitializeICU()) {
			v8::Platform* platform = v8::platform::CreateDefaultPlatform();
			v8::V8::InitializePlatform(platform);
			if (v8::V8::Initialize()) {
				Isolate::CreateParams create_params;
				create_params.array_buffer_allocator = &allocator;
				isolate = Isolate::New(create_params);

			}
		}
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
    Handle<ObjectTemplate> console = JS_NEW_OBJECT_TEMPLATE();
    console->Set(JS_NEW_STRING("log"),
            JS_NEW_FUNCTION_TEMPLATE(_OPscriptV8Log));

    objTemplate->Set(JS_NEW_STRING("console"), console);
}

OPchar* _OPscriptV8GetDir(const OPchar* path) {
    OPchar* result = OPstringCopy("");
    OPint pos = -1;
    for(OPint i = 0; i < strlen(path); i++) {
        if(path[i] == '/') {
            pos = i;
        }
    }
    if(pos > -1) {
        return OPstringSub(path, 0, pos);
    } else {
        return OPstringCopy("");
    }
}


Handle<Value> _OPjavaScriptRequireScript(OPscript* script, OPchar* dir) {
    OPjavaScriptV8Compiled compiled;
    OPjavaScriptV8Compile(&compiled, script, dir);

    OPjavaScriptV8Run(&compiled);
    Handle<Context> localContext = Local<Context>::New(isolate, compiled.Context);

    localContext->DetachGlobal();
    Local<Object> global = localContext->Global();
    Local<Value> moduleVal = global->Get(JS_NEW_STRING("module"));
    Local<Object> module = moduleVal->ToObject();
    Handle<Value> exports = module->Get(JS_NEW_STRING("exports"));
    return exports;
}

OPint loadCount = 0;
Persistent<Value, CopyablePersistentTraits<Value> > loaded[CACHED_FILE_COUNT];
#ifdef _DEBUG
OPscript* loadedScripts[CACHED_FILE_COUNT];
#endif
OPchar* stored[CACHED_FILE_COUNT];

Handle<Value> _OPjavaScriptRequire(OPchar* pathToLoad) {
    OPlog("Loading '%s' for Require", pathToLoad);

    for(OPint i = 0; i < loadCount; i++) {
        if(OPstringEquals(pathToLoad, stored[i])) {
            OPlog("Reloaded %s", pathToLoad);

			#ifdef _DEBUG
			if(loadedScripts[i]->changed) {
				// The script was changed, releoad the cache
			    OPchar* tmpDir = _OPscriptV8GetDir(pathToLoad);
				Handle<Value> tmpResult = _OPjavaScriptRequireScript(loadedScripts[i], tmpDir);
				loaded[loadCount] = Persistent<Value, CopyablePersistentTraits<Value> >(isolate, tmpResult);
				return tmpResult;
			}
			#endif
            return Local<Value>::New(isolate, loaded[i]);
        }
    }

    ASSERT(loadCount < CACHED_FILE_COUNT, "Too many cached files. Increase the CACHED_FILE_COUNT define in OPjavaScriptV8.cpp");

    OPscript* scriptSource = (OPscript *) OPcmanLoadGet(pathToLoad);

    OPchar *dir = _OPscriptV8GetDir(pathToLoad);

    Handle<Value> result = _OPjavaScriptRequireScript(scriptSource, dir);


    loaded[loadCount] = Persistent<Value, CopyablePersistentTraits<Value> >(isolate, result);
    stored[loadCount] = OPstringCopy(pathToLoad);
	#ifdef _DEBUG
	loadedScripts[loadCount] = scriptSource;
	#endif
    loadCount++;

    OPfree(dir);

    return result;
}


#include "./Scripting/include/JavaScript/Core/Wrappers.h"
#include "./Scripting/include/JavaScript/Data/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"
#include "./Scripting/include/JavaScript/Performance/Wrappers.h"
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Communication/Wrappers.h"
#include "./Scripting/include/JavaScript/Pipeline/Wrappers.h"

OPint setup = 0;
Persistent<Object, CopyablePersistentTraits<Object> > exports;

Handle<Object> _OPscriptV8WrapEngine() {

    if (!setup) {

		Handle<Object> Wrapper = JS_NEW_OBJECT();

        Handle<Object> OP = JS_NEW_OBJECT();

        OPscriptNodeWrapperCore(OP);
        OPscriptNodeWrapperData(OP);
        OPscriptNodeWrapperMath(OP);
        OPscriptNodeWrapperPerformance(OP);
        OPscriptNodeWrapperHuman(OP);
        OPscriptNodeWrapperCommunication(OP);
        OPscriptNodeWrapperPipeline(OP);

        OPlog("Wrapped the engine.");

        //OPscript *result = NULL;
        //OPscriptLoad("Scripts/node_modules/OPengine/OPgameState.js", &result);
        //OPlog("Loaded OPgameState.js Script.");
        //JS_SET_OBJECT(tmp, "gameState", _OPjavaScriptRequireScript(result, NULL));

        //OPlog("Wrapped OPgameState.");

		// If the application specified a Custom Wrapper
		// Then we call it and it sets anyrthing it needs
		// onto the Wrapper obj
		if(OPJAVASCRIPTV8_CUSTOMWRAPPER != NULL) {
			OPJAVASCRIPTV8_CUSTOMWRAPPER(Wrapper);
		}

		JS_SET_OBJECT(Wrapper, "OP", OP);

        exports = Persistent<Object, CopyablePersistentTraits<Object> >(isolate, Wrapper);
        setup = 1;
    }

    return Local<Object>::New(isolate, exports);
}

JS_RETURN_VAL _platform(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    #if defined(OPIFEX_WINDOWS)
        JS_RETURN(JS_NEW_STRING("win32"));
    #elif defined(OPIFEX_OSX)
        JS_RETURN(JS_NEW_STRING("darwin"));
    #else
        JS_RETURN(JS_NEW_STRING("unix"));
    #endif

    JS_RETURN_NULL;
}

JS_RETURN_VAL _arch(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    #if defined(OPIFEX_OS64)
        JS_RETURN(JS_NEW_STRING("x64"));
    #else
        JS_RETURN(JS_NEW_STRING("x86"));
    #endif

    JS_RETURN_NULL;
}

OPint setupOS = 0;
Persistent<Object, CopyablePersistentTraits<Object> > exportsOS;
Handle<Object> _OPscriptV8WrapOS() {

    if (!setupOS) {

        Handle<Object> tmp = JS_NEW_OBJECT();

        JS_SET_METHOD(tmp, "platform", _platform);
        JS_SET_METHOD(tmp, "arch", _arch);

        exportsOS = Persistent<Object, CopyablePersistentTraits<Object> >(isolate, tmp);
        setupOS = 1;
    }

    return Local<Object>::New(isolate, exportsOS);
}

OPchar* _OPscriptV8NormalizePath(const OPchar* path) {

    // Get a copy of the current argument path that we can use to alter it
    OPchar* pathCopy = OPstringCopy(path);
    OPchar* pathToLoad = pathCopy;

    const OPchar* begin = "./";
    const OPchar* jsEnd = ".js";
    const OPchar* opsEnd = ".ops";

    // If the require starts with a ./ then remove it
    if(OPstringStartsWith(pathToLoad, "./")) {
        OPchar* temp = OPstringCopy(&pathToLoad[2]);
        OPfree(pathToLoad);
        pathToLoad = temp;
    }

    // If the require doesn't end with a .js or a .ops then load from node_modules

    if (!OPstringEndsWith(pathToLoad, jsEnd) && !OPstringEndsWith(pathToLoad, opsEnd)) {
        OPchar* start = OPstringCreateMerged("node_modules/", pathToLoad);
        OPchar* result = OPstringCreateMerged(start, "/index.js");
        OPfree(start);
        OPfree(pathToLoad);
        pathToLoad = result;
    }


    return pathToLoad;
}


void _OPscriptV8Require(const v8::FunctionCallbackInfo<v8::Value>& args) {
    SCOPE_AND_ISOLATE;

    if (args.Length() == 0 || !args[0]->IsString()) return;

    v8::String::Utf8Value utf8(args[0]);
    const char *arg0 = *utf8;

    if (OPstringEquals(arg0, "OPengine")) {
        args.GetReturnValue().Set(_OPscriptV8WrapEngine());
        return;
    }
    if (OPstringEquals(arg0, "os")) {
        args.GetReturnValue().Set(_OPscriptV8WrapOS());
        return;
    }


    Handle<Object> currGlobal = args.Callee()->CreationContext()->Global();
    Handle<Value> __dirname = currGlobal->Get(JS_NEW_STRING("__dirname"));

    String::Utf8Value str(__dirname->ToString());
    //OPlog("__dirname %s", *str);

    if (OPJAVASCRIPTV8_REQUIRE == NULL || !OPJAVASCRIPTV8_REQUIRE(args)) {
        OPchar *pathToLoad = _OPscriptV8NormalizePath(arg0);
        args.GetReturnValue().Set(_OPjavaScriptRequire(pathToLoad));
        OPfree(pathToLoad);
    }

}
void _OPscriptV8SetImmediate(const v8::FunctionCallbackInfo<v8::Value>& args) {
    SCOPE_AND_ISOLATE;

    if (args.Length() == 0) return;

    Handle<Object> obj = JS_NEW_OBJECT();
    Handle<Value> argv[1] = { JS_NEW_BOOL(true) };
    Local<Function> callback = Local<Function>::Cast(args[0]);
    callback->Call(obj, 1, argv);
}

OPint OPjavaScriptV8Compile(OPjavaScriptV8Compiled* compiled, OPscript* script, OPchar* dir) {
	ASSERT(isolate != NULL, "V8 Engine must be initialized first.");

	SCOPE_AND_ISOLATE;

	Handle<ObjectTemplate> global = JS_NEW_OBJECT_TEMPLATE();
	global->Set(JS_NEW_STRING("module"), JS_NEW_OBJECT_TEMPLATE());
	global->Set(JS_NEW_STRING("global"), JS_NEW_OBJECT_TEMPLATE());
    global->Set(JS_NEW_STRING("require"), JS_NEW_FUNCTION_TEMPLATE(_OPscriptV8Require));
    global->Set(JS_NEW_STRING("setImmediate"), JS_NEW_FUNCTION_TEMPLATE(_OPscriptV8SetImmediate));
	_OPscriptV8SetConsole(global);

	Handle<Context> localContext = Context::New(isolate, NULL, global);

	v8::Context::Scope context_scope(localContext);


    Local<Object> globalSelf = localContext->Global();
    if(dir == NULL) {
        JS_SET_STRING(globalSelf, "__dirname", "");
    } else {
        JS_SET_STRING(globalSelf, "__dirname", dir);
    }


	Handle<String> source = JS_NEW_STRING(script->data);

	v8::ScriptOrigin origin(JS_NEW_STRING("name"));
	Local<Script> compiledV8 = v8::Script::Compile(source, &origin);
	if (compiledV8.IsEmpty()) {
		return 0;
	}

    OPjavaScriptV8Compiled result = {
		script,
		Persistent<Script, CopyablePersistentTraits<Script> >(isolate, compiledV8),
		Persistent<Context, CopyablePersistentTraits<Context> >(isolate, localContext),
	};

	*compiled = result;
	return 1;
}


void OPjavaScriptV8Update(OPjavaScriptV8Compiled* scriptCompiled) {
//    if (scriptCompiled->Source->changed) {
//        OPjavaScriptV8Compiled temp;
//        if (OPjavaScriptV8Compiled(&temp, scriptCompiled->Source, &scriptCompiled->Context)) {
//            *scriptCompiled = temp;
//            _runCompiled(scriptCompiled);
//        }
//        scriptCompiled->Source->changed = 0;
//    }
}

OPjavaScriptPersistentValue OPjavaScriptV8Run(OPjavaScriptV8Compiled* scriptCompiled) {
    SCOPE_AND_ISOLATE;

    Handle<Context> context = Local<Context>::New(isolate, scriptCompiled->Context);
    v8::Context::Scope context_scope(context);

    Handle<Script> compiled = Local<Script>::New(isolate, scriptCompiled->Script);

    TryCatch trycatch;
    Local<Value> result = compiled->Run();
    if (result.IsEmpty()) {
        Local<Value> exception = trycatch.Exception();
        String::Utf8Value exception_str(exception);
        OPlog("OPscriptV8: Exception: %s", *exception_str);

        return Persistent<Value>(isolate, JS_NEW_NULL());
    }

    return Persistent<Value>(isolate, result);
}

OPjavaScriptPersistentValue OPjavaScriptV8Run(OPjavaScriptV8Compiled* scriptCompiled, OPchar* name) {
    SCOPE_AND_ISOLATE;

    Handle<Context> context = Local<Context>::New(isolate, scriptCompiled->Context);
    v8::Context::Scope context_scope(context);
    Handle<v8::Object> global = context->Global();
    Handle<v8::Value> value = global->Get(JS_NEW_STRING(name));
    Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(value);
    return Persistent<Value>(isolate, func->Call(global, 0, NULL));

//    Handle<Value> values[10];
//    for (OPint i = 0; i < count; i++) {
//        values[i] = Local<Value>::New(OPSCRIPTV8_ISOLATE, args[i]);
//    }

}

OPjavaScriptPersistentValue OPjavaScriptV8Run(OPjavaScriptV8Compiled* scriptCompiled, OPchar* name, OPuint count, OPjavaScriptPersistentValue* args) {
    SCOPE_AND_ISOLATE;

    Handle<Context> context = Local<Context>::New(isolate, scriptCompiled->Context);
    v8::Context::Scope context_scope(context);
    Handle<v8::Object> global = context->Global();
    Handle<v8::Value> value = global->Get(JS_NEW_STRING(name));
    Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(value);

    Handle<Value> values[10];
    for (OPint i = 0; i < count; i++) {
        values[i] = Local<Value>::New(isolate, args[i]);
    }

    return Persistent<Value>(isolate, func->Call(global, count, values));
}

void OPjavaScriptV8SetupRun(const OPchar* script) {
    OPjavaScriptV8Init();
    OPscript *result = NULL;
    OPscriptLoad(script, &result);
    OPjavaScriptV8Compiled compiled;
    OPjavaScriptV8Compile(&compiled, result);
    OPjavaScriptV8Run(&compiled);
}

#endif

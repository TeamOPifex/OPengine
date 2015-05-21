#include "./Scripting/include/JavaScript/OPjavaScriptV8.h"
#include "./Scripting/include/JavaScript/OPjavaScriptHelper.h"

#ifdef OPIFEX_OPTION_V8

#include "./Core/include/Assert.h"
#include "./include/libplatform/libplatform.h"
#include "./Data/include/OPstring.h"
#include "./Data/include/OPcman.h"

Isolate* isolate = NULL;

void OPjavaScriptV8Init() {
	if(isolate == NULL) {
		v8::V8::InitializeICU();
		v8::Platform* platform = v8::platform::CreateDefaultPlatform();
		v8::V8::InitializePlatform(platform);
		v8::V8::Initialize();
		isolate = Isolate::New();
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
Handle<Value> _OPjavaScriptRequire(OPchar* pathToLoad) {
    OPlog("Loading '%s' for Require", pathToLoad);

    OPscript* scriptSource = (OPscript*)OPcmanLoadGet(pathToLoad);

    OPchar* dir = _OPscriptV8GetDir(pathToLoad);

    OPjavaScriptV8Compiled compiled;
    OPjavaScriptV8Compile(&compiled, scriptSource, dir);

    OPfree(dir);

    OPjavaScriptV8Run(&compiled);
    Handle<Context> localContext = Local<Context>::New(isolate, compiled.Context);

    localContext->DetachGlobal();
    Local<Object> global = localContext->Global();
    Local<Value> moduleVal = global->Get(JS_NEW_STRING("module"));
    Local<Object> module = moduleVal->ToObject();
    Handle<Value> exports = module->Get(JS_NEW_STRING("exports"));
    return exports;
}


#include "./Scripting/include/JavaScript/Core/Wrappers.h"
#include "./Scripting/include/JavaScript/Data/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"
#include "./Scripting/include/JavaScript/Performance/Wrappers.h"
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Communication/Wrappers.h"
#include "./Scripting/include/JavaScript/Pipeline/Wrappers.h"

void _OPscriptV8WrapEngine(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Handle<Object> exports = JS_NEW_OBJECT();

    OPscriptNodeWrapperCore(exports);
    OPscriptNodeWrapperData(exports);
    OPscriptNodeWrapperMath(exports);
    OPscriptNodeWrapperPerformance(exports);
    OPscriptNodeWrapperHuman(exports);
    OPscriptNodeWrapperCommunication(exports);
    OPscriptNodeWrapperPipeline(exports);

    JS_SET_OBJECT(exports, "gameState", _OPjavaScriptRequire("node_modules/OPengine/OPgameState.js"));

    args.GetReturnValue().Set(exports);
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
    const char* arg0 = *utf8;

    if (OPstringEquals(arg0, "OPengine")) {
        _OPscriptV8WrapEngine(args);
        return;
    }



    Handle<Object> currGlobal = args.Callee()->CreationContext()->Global();
    Handle<Value> __dirname = currGlobal->Get(JS_NEW_STRING("__dirname"));

    String::Utf8Value str(__dirname->ToString());
    //OPlog("__dirname %s", *str);

    //if (OPSCRIPTV8_REQUIRE == NULL || !OPSCRIPTV8_REQUIRE(args)) {

    OPchar* pathToLoad = _OPscriptV8NormalizePath(arg0);
    args.GetReturnValue().Set(_OPjavaScriptRequire(pathToLoad));
    OPfree(pathToLoad);


//
//        OPchar* dir = _OPscriptV8GetDir(pathToLoad);
//        OPlog("Curr Path %s", dir);
//
//
//
//        if(OPcmanLoad(pathToLoad) == 0) return;
//
//        OPscript* scriptSource = (OPscript*)OPcmanGet(pathToLoad);
//
//        OPfree(pathToLoad);
//
//        OPjavaScriptV8Compiled compiled;
//        OPjavaScriptV8Compile(&compiled, scriptSource, dir);
//        OPfree(dir);
//
//        //Local<Value> result =
//        OPjavaScriptV8Run(&compiled);
//        Handle<Context> localContext = Local<Context>::New(isolate, compiled.Context);
//
//        localContext->DetachGlobal();
//        Local<Object> global = localContext->Global();
//        Local<Value> moduleVal = global->Get(JS_NEW_STRING("module"));
//        if(!moduleVal->IsNull()) {
//            Local<Object> module = moduleVal->ToObject();
//            Local<Value> exports = module->Get(JS_NEW_STRING("exports"));
//
//            args.GetReturnValue().Set(exports);
//        }


    //}
}

OPint OPjavaScriptV8Compile(OPjavaScriptV8Compiled* compiled, OPscript* script, OPchar* dir) {
	ASSERT(isolate != NULL, "V8 Engine must be initialized first.");

	SCOPE_AND_ISOLATE;

	Handle<ObjectTemplate> global = JS_NEW_OBJECT_TEMPLATE();
	global->Set(JS_NEW_STRING("module"), JS_NEW_OBJECT_TEMPLATE());
	global->Set(JS_NEW_STRING("global"), JS_NEW_OBJECT_TEMPLATE());
	global->Set(JS_NEW_STRING("require"), JS_NEW_FUNCTION_TEMPLATE(_OPscriptV8Require));
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
	Handle<Script> compiledV8 = v8::Script::Compile(source, &origin);
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

void OPjavaScriptV8Run(OPjavaScriptV8Compiled* scriptCompiled) {
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
    }
}

#endif
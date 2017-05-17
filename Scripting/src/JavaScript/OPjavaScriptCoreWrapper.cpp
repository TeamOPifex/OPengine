#include "./Scripting/include/JavaScript/OPjavaScriptHelper.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Scripting/include/JavaScript/Core/Wrappers.h"

#include "./Core/include/OPlog.h"
#include "./Core/Core.h"

Local<Function> _initCallback;
Local<Function> _updateCallback;
Local<Function> _renderCallback;
Local<Function> _endCallback;

void _init() {
    SCOPE_AND_ISOLATE
    const unsigned int argc = 1;
    Handle<Value> argv[argc] = { JS_NEW_BOOL(true) };
    Handle<Object> obj = JS_NEW_OBJECT();

    #ifdef OPIFEX_OPTION_V8
    	TryCatch trycatch;
        Local<Value> result = _initCallback->Call(obj, argc, argv);
        if (result.IsEmpty()) {
    		ReportException(isolate, &trycatch);
        }
    #else
        Local<Value> result = _initCallback->Call(obj, argc, argv);
    #endif
}

OPint _update(OPtimer* timer) {
    SCOPE_AND_ISOLATE
    const unsigned int argc = 1;
    Handle<Object> timerObj = JS_NEW_OBJECT();
    JS_SET_PTR(timerObj, timer);
    JS_SET_NUMBER(timerObj, "elapsed", timer->Elapsed);

    Handle<Value> argv[argc] = { timerObj };
    Handle<Object> obj = JS_NEW_OBJECT();

    #ifdef OPIFEX_OPTION_V8
    	TryCatch trycatch;
        Local<Value> result = _updateCallback->Call(obj, argc, argv);

        if (result.IsEmpty()) {
    		ReportException(isolate, &trycatch);
            return 1;
        } else if(result->IsNumber()) {
            OPint retVal = result->Int32Value();
            return retVal;
        }
    #else
        Local<Value> result = _updateCallback->Call(obj, argc, argv);
        if(result->IsNumber()) {
            OPint retVal = result->Int32Value();
            return retVal;
        }
    #endif
    return 0;
}

void _render(OPfloat delta) {
    SCOPE_AND_ISOLATE
    const unsigned int argc = 1;

    Handle<Value> argv[argc] = { JS_NEW_NUMBER(delta) };
    Handle<Object> obj = JS_NEW_OBJECT();

	TryCatch trycatch;
    _renderCallback->Call(obj, argc, argv);
}

void _end() {
    SCOPE_AND_ISOLATE
    const unsigned int argc = 1;
    Handle<Value> argv[argc] = { JS_NEW_BOOL(true) };
    Handle<Object> obj = JS_NEW_OBJECT();

    #ifdef OPIFEX_OPTION_V8
    	TryCatch trycatch;
        Local<Value> result = _endCallback->Call(obj, argc, argv);
        if (result.IsEmpty()) {
    		ReportException(isolate, &trycatch);
        }
    #else
        Local<Value> result = _endCallback->Call(obj, argc, argv);
    #endif
}

JS_RETURN_VAL _OPstart(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPlog("Starting");

    _initCallback = Local<Function>::Cast(args[0]);
    _updateCallback = Local<Function>::Cast(args[1]);
    _renderCallback = Local<Function>::Cast(args[2]);
    _endCallback = Local<Function>::Cast(args[3]);

 	OPinitialize = _init;
 	OPupdate = _update;
    OPrender = _render;
 	OPdestroy = _end;

    OPstart(0, NULL);
    OPlog("OPstart finished");
    OPend();

    JS_RETURN_NULL
}

JS_RETURN_VAL _OPstartStepped(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPlog("Starting");

    _initCallback = Local<Function>::Cast(args[0]);
    _updateCallback = Local<Function>::Cast(args[1]);
    _renderCallback = Local<Function>::Cast(args[2]);
    _endCallback = Local<Function>::Cast(args[3]);

 	OPinitialize = _init;
 	OPupdate = _update;
    OPrender = _render;
 	OPdestroy = _end;

    OPstartStepped(0, NULL);
    OPlog("OPstart finished");
    OPend();

    JS_RETURN_NULL
}


// OP.alloc
JS_RETURN_VAL _OPalloc(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

 	void* data = OPalloc(args[0]->IntegerValue());

    Handle<Object> obj = JS_NEW_OBJECT();
    JS_SET_PTR(obj, data);

    JS_RETURN(obj);
}

// OP.allocZero
JS_RETURN_VAL _OPallocZero(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

 	void* data = OPallocZero(args[0]->IntegerValue());

    Handle<Object> obj = JS_NEW_OBJECT();
    JS_SET_PTR(obj, data);

    JS_RETURN(obj);
}

// OP.free
JS_RETURN_VAL _OPfree(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

 	void* data = JS_GET_ARG_PTR(args, 0, void);
 	OPfree(data);

 	JS_RETURN_NULL
}

// OP.log
JS_RETURN_VAL _OPlog(const JS_ARGS& args) {
	SCOPE_AND_ISOLATE

		String::Utf8Value str(args[0]->ToString());
	OPlog(*str);

	JS_RETURN_NULL
}

// OP.logError
JS_RETURN_VAL _OPlogError(const JS_ARGS& args) {
	SCOPE_AND_ISOLATE

		String::Utf8Value str(args[0]->ToString());
	OPlogErr(*str);

	JS_RETURN_NULL
}


// Initialize the Core
void OPscriptNodeWrapperCore(Handle<Object> exports) {
    SCOPE_AND_ISOLATE

    OPtimerWrapper(exports);
    OPdefinedWrapper(exports);

    JS_SET_METHOD(exports, "start", _OPstart);
    JS_SET_METHOD(exports, "startStepped", _OPstartStepped);
    JS_SET_METHOD(exports, "alloc", _OPalloc);
    JS_SET_METHOD(exports, "allocZero", _OPallocZero);
	JS_SET_METHOD(exports, "log", _OPlog);
	JS_SET_METHOD(exports, "error", _OPlogError);
    JS_SET_METHOD(exports, "free", _OPfree);
}

#endif

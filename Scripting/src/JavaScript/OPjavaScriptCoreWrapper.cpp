#include "./Scripting/include/JavaScript/OPjavaScriptHelper.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Scripting/include/JavaScript/Core/Wrappers.h"

#include "./Core/include/OPlog.h"
#include "./Core/Core.h"

Local<Function> _initCallback;
Local<Function> _updateCallback;
Local<Function> _endCallback;

void _init() {
    SCOPE_AND_ISOLATE
    const unsigned int argc = 1;
    Handle<Value> argv[argc] = { JS_NEW_BOOL(true) };
    Handle<Object> obj = JS_NEW_OBJECT();
    _initCallback->Call(obj, argc, argv);
}

int _update(OPtimer* timer) {
    SCOPE_AND_ISOLATE
    const unsigned int argc = 1;
    Handle<Value> argv[argc] = { JS_NEW_NUMBER(timer->Elapsed) };
    Handle<Object> obj = JS_NEW_OBJECT();
    Local<Value> result = _updateCallback->Call(obj, argc, argv);
    if(result->IsNumber()) {
        OPint retVal = result->Int32Value();
        OPlog("Update %d", retVal);
        return retVal;
    }
    return 0;
}

void _end() {
    SCOPE_AND_ISOLATE
    const unsigned int argc = 1;
    Handle<Value> argv[argc] = { JS_NEW_BOOL(true) };
    Handle<Object> obj = JS_NEW_OBJECT();
    _endCallback->Call(obj, argc, argv);
}

JS_RETURN_VAL _OPstart(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPlog("Starting");

    _initCallback = Local<Function>::Cast(args[0]);
    _updateCallback = Local<Function>::Cast(args[1]);
    _endCallback = Local<Function>::Cast(args[2]);

 	OPinitialize = _init;
 	OPupdate = _update;
 	OPdestroy = _end;

    OPstart(0, NULL);
    OPlog("OPstart finished");
    OPend();

    JS_RETURN_NULL
}


// OP.alloc
JS_RETURN_VAL _OPalloc(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

 	void* data = OPalloc(args[0]->NumberValue());

    Handle<Object> obj = JS_NEW_OBJECT();
    JS_SET_PTR(obj, data);

    JS_RETURN(obj);
}

// OP.allocZero
JS_RETURN_VAL _OPallocZero(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

 	void* data = OPallocZero(args[0]->NumberValue());

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


// Initialize the Core
void OPscriptNodeWrapperCore(Handle<Object> exports) {
    SCOPE_AND_ISOLATE

    OPtimerWrapper(exports);
    OPdefinedWrapper(exports);

    JS_SET_METHOD(exports, "start", _OPstart);
    JS_SET_METHOD(exports, "alloc", _OPalloc);
    JS_SET_METHOD(exports, "allocZero", _OPallocZero);
    JS_SET_METHOD(exports, "free", _OPfree);
}

#endif
#include "./Scripting/include/Node/OPscriptNodeHelper.h"

#ifdef OPIFEX_OPTION_NODEJS

#include "./Scripting/include/Node/OPscriptNodeWrapperCore.h"

#include "./Core/include/OPlog.h"
#include "./Core/Core.h"

Local<Function> _initCallback;
Local<Function> _updateCallback;
Local<Function> _endCallback;

void _init() {
    SCOPE_AND_ISOLATE
    const unsigned int argc = 1;
    Handle<Value> argv[argc] = { Boolean::New(isolate, true) };
    Handle<Object> obj = NODE_NEW_OBJECT();
    _initCallback->Call(obj, argc, argv);
}

int _update(OPtimer* timer) {
    SCOPE_AND_ISOLATE
    const unsigned int argc = 1;
    Handle<Value> argv[argc] = { Number::New(isolate, timer->Elapsed) };
    Handle<Object> obj = NODE_NEW_OBJECT();
    Local<Value> result = _updateCallback->Call(obj, argc, argv);
    if(result->IsNumber())
        return result->Int32Value();
    return 0;
}

void _end() {
    SCOPE_AND_ISOLATE
    const unsigned int argc = 1;
    Handle<Value> argv[argc] = { Boolean::New(isolate, true) };
    Handle<Object> obj = NODE_NEW_OBJECT();
    _endCallback->Call(obj, argc, argv);
}

void _OPstart(const FunctionCallbackInfo<Value>& args) {
    SCOPE_AND_ISOLATE

    OPlog("Starting");

    _initCallback = Local<Function>::Cast(args[0]);
    _updateCallback = Local<Function>::Cast(args[1]);
    _endCallback = Local<Function>::Cast(args[2]);

 	OPinitialize = _init;
 	OPupdate = _update;
 	OPdestroy = _end;

    OPstart(0, NULL);
    OPend();

    args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world"));
}

void OPscriptNodeWrapperCore(Handle<Object> exports) {
    SCOPE_AND_ISOLATE

    Handle<Object> core = Object::New(isolate);

    exports->Set(String::NewFromUtf8(isolate, "core"), core);


    NODE_SET_METHOD(exports, "start", _OPstart);
}

#endif
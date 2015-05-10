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
    OPlog("OPstart finished");
    OPend();
}

// OP.timer.Create
void _OPtimerCreate(const FunctionCallbackInfo<Value>& args) {
    SCOPE_AND_ISOLATE

 	OPtimer* timer = (OPtimer*)OPalloc(sizeof(OPtimer));
 	OPtimerCreate(timer);

    Handle<Object> obj = Object::New(isolate);
    obj->Set(String::NewFromUtf8(isolate, "ptr"), Number::New(isolate, (OPint)timer));
    obj->Set(String::NewFromUtf8(isolate, "elapsed"), Number::New(isolate, 0));

    args.GetReturnValue().Set(obj);
}

// OP.timer.Update
void _OPtimerUpdate(const FunctionCallbackInfo<Value>& args) {
    SCOPE_AND_ISOLATE

    Handle<Object> obj = args[0]->ToObject();
    OPint ptr = (OPint)obj->Get(String::NewFromUtf8(isolate, "ptr"))->IntegerValue();
 	OPtimer* timer = (OPtimer*)(OPint)ptr;
 	OPtimerTick(timer);
    obj->Set(String::NewFromUtf8(isolate, "elapsed"), Number::New(isolate, timer->Elapsed));
}

// Initialize the Core
void OPscriptNodeWrapperCore(Handle<Object> exports) {
    SCOPE_AND_ISOLATE

    {
        // OP.timer
        Handle<Object> timer = Object::New(isolate);
        NODE_SET_METHOD(timer, "Create", _OPtimerCreate);
        NODE_SET_METHOD(timer, "Update", _OPtimerUpdate);
        exports->Set(String::NewFromUtf8(isolate, "timer"), timer);
    }



    NODE_SET_METHOD(exports, "start", _OPstart);
}

#endif
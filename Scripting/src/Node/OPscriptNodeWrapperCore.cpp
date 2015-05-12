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
    Handle<Value> argv[argc] = { NODE_NEW_BOOL(true) };
    Handle<Object> obj = NODE_NEW_OBJECT();
    _initCallback->Call(obj, argc, argv);
}

int _update(OPtimer* timer) {
    SCOPE_AND_ISOLATE
    const unsigned int argc = 1;
    Handle<Value> argv[argc] = { NODE_NEW_NUMBER(timer->Elapsed) };
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
    Handle<Value> argv[argc] = { NODE_NEW_BOOL(true) };
    Handle<Object> obj = NODE_NEW_OBJECT();
    _endCallback->Call(obj, argc, argv);
}

NODE_RETURN_VAL _OPstart(const NODE_ARGS& args) {
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

    NODE_RETURN_NULL
}

// OP.timer.Create
NODE_RETURN_VAL _OPtimerCreate(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

 	OPtimer* timer = (OPtimer*)OPalloc(sizeof(OPtimer));
 	OPtimerCreate(timer);

    Handle<Object> obj = NODE_NEW_OBJECT();
    NODE_SET_PTR(obj, timer)
    NODE_SET_NUMBER(obj, "ptr", (OPint)timer);
    NODE_SET_NUMBER(obj, "elapsed", 0);

    NODE_RETURN(obj);
}

// OP.timer.Update
NODE_RETURN_VAL _OPtimerUpdate(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    Handle<Object> obj = args[0]->ToObject();

    OPtimer* timer = NODE_GET_PTR(obj, OPtimer);
 	OPtimerTick(timer);

    NODE_SET_NUMBER(obj, "elapsed", timer->Elapsed);

    NODE_RETURN_NULL
}

// Initialize the Core
void OPscriptNodeWrapperCore(Handle<Object> exports) {
    SCOPE_AND_ISOLATE

    {
        // OP.timer
        Handle<Object> timer = NODE_NEW_OBJECT();
        NODE_SET_METHOD(timer, "Create", _OPtimerCreate);
        NODE_SET_METHOD(timer, "Update", _OPtimerUpdate);
        NODE_SET_OBJECT(exports, "timer", timer);
    }

    NODE_SET_METHOD(exports, "start", _OPstart);
}

#endif
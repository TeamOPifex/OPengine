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

// OP.alloc
NODE_RETURN_VAL _OPalloc(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

 	void* data = OPalloc(args[0]->NumberValue());

    Handle<Object> obj = NODE_NEW_OBJECT();
    NODE_SET_PTR(obj, data);

    NODE_RETURN(obj);
}

// OP.allocZero
NODE_RETURN_VAL _OPallocZero(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

 	void* data = OPallocZero(args[0]->NumberValue());

    Handle<Object> obj = NODE_NEW_OBJECT();
    NODE_SET_PTR(obj, data);

    NODE_RETURN(obj);
}

// OP.free
NODE_RETURN_VAL _OPfree(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

 	void* data = NODE_GET_ARG_PTR(args, 0, void);
 	OPfree(data);

 	NODE_RETURN_NULL
}

void _SetDefines(Handle<Object> obj) {
    SCOPE_AND_ISOLATE

    #ifdef OPIFEX_OPTION_MYO
        NODE_SET_NUMBER(obj, "OPIFEX_OPTION_MYO", 1);
    #endif
    #ifdef OPIFEX_OPTION_OCULUS
        NODE_SET_NUMBER(obj, "OPIFEX_OPTION_OCULUS", 1);
    #endif
    #ifdef OPIFEX_OPTION_PHYSICS
        NODE_SET_NUMBER(obj, "OPIFEX_OPTION_PHYSICS", 1);
    #endif
    #ifdef OPIFEX_OPTION_RELEASE
        NODE_SET_NUMBER(obj, "OPIFEX_OPTION_RELEASE", 1);
    #endif
    #ifdef OPIFEX_OPTION_SHARED
        NODE_SET_NUMBER(obj, "OPIFEX_OPTION_SHARED", 1);
    #endif
    #ifdef OPIFEX_OPTION_SPINE
        NODE_SET_NUMBER(obj, "OPIFEX_OPTION_SPINE", 1);
    #endif
    #ifdef OPIFEX_OPTION_FMOD
        NODE_SET_NUMBER(obj, "OPIFEX_OPTION_FMOD", 1);
    #endif
    #ifdef OPIFEX_OPTION_AUDIO
        NODE_SET_NUMBER(obj, "OPIFEX_OPTION_AUDIO", 1);
    #endif
    #ifdef OPIFEX_OPTION_V8
        NODE_SET_NUMBER(obj, "OPIFEX_OPTION_V8", 1);
    #endif
    #ifdef OPIFEX_GL_2_0
        NODE_SET_NUMBER(obj, "OPIFEX_GL_2_0", 1);
    #endif
    #ifdef OPIFEX_GL_3_3
        NODE_SET_NUMBER(obj, "OPIFEX_GL_3_3", 1);
    #endif

    #ifdef OPIFEX_OSX32
        NODE_SET_NUMBER(obj, "OPIFEX_OSX32", 1);
    #endif
    #ifdef OPIFEX_OSX64
        NODE_SET_NUMBER(obj, "OPIFEX_OSX64", 1);
    #endif
    #ifdef OPIFEX_OSX
        NODE_SET_NUMBER(obj, "OPIFEX_OSX", 1);
    #endif
    #ifdef OPIFEX_LINUX32
        NODE_SET_NUMBER(obj, "OPIFEX_LINUX32", 1);
    #endif
    #ifdef OPIFEX_LINUX64
        NODE_SET_NUMBER(obj, "OPIFEX_LINUX64", 1);
    #endif
    #ifdef OPIFEX_LINUX
        NODE_SET_NUMBER(obj, "OPIFEX_LINUX", 1);
    #endif
    #ifdef OPIFEX_WIN32
        NODE_SET_NUMBER(obj, "OPIFEX_WIN32", 1);
    #endif
    #ifdef OPIFEX_WIN64
        NODE_SET_NUMBER(obj, "OPIFEX_WIN64", 1);
    #endif
    #ifdef OPIFEX_WINDOWS
        NODE_SET_NUMBER(obj, "OPIFEX_WINDOWS", 1);
    #endif
    #ifdef OPIFEX_ANDROID
        NODE_SET_NUMBER(obj, "OPIFEX_ANDROID", 1);
    #endif
    #ifdef OPIFEX_UNIX
        NODE_SET_NUMBER(obj, "OPIFEX_UNIX", 1);
    #endif
    #ifdef OPIFEX_OS32
        NODE_SET_NUMBER(obj, "OPIFEX_OS32", 1);
    #endif
    #ifdef OPIFEX_OS64
        NODE_SET_NUMBER(obj, "OPIFEX_OS64", 1);
    #endif
}

// Initialize the Core
void OPscriptNodeWrapperCore(Handle<Object> exports) {
    SCOPE_AND_ISOLATE

    { // OP.timer
        Handle<Object> timer = NODE_NEW_OBJECT();
        NODE_SET_METHOD(timer, "Create", _OPtimerCreate);
        NODE_SET_METHOD(timer, "Update", _OPtimerUpdate);
        NODE_SET_OBJECT(exports, "timer", timer);
    }

    { // OP.defined
        Handle<Object> defined = NODE_NEW_OBJECT();
        _SetDefines(defined);
        NODE_SET_OBJECT(exports, "defined", defined);
    }

    NODE_SET_METHOD(exports, "start", _OPstart);
    NODE_SET_METHOD(exports, "alloc", _OPalloc);
    NODE_SET_METHOD(exports, "allocZero", _OPallocZero);
    NODE_SET_METHOD(exports, "free", _OPfree);
}

#endif
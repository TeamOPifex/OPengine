// myobject.cc
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Core/Core.h"

Handle<Object> OPtimerWrapper(Handle<Object> result, OPtimer* ptr) {
    SCOPE_AND_ISOLATE

    JS_SET_PTR(result, ptr);

    return result;
}

// OP.timer.Create
JS_RETURN_VAL _OPtimerCreate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPtimer* timer = (OPtimer*)OPalloc(sizeof(OPtimer));
	timer->Init();

    Handle<Object> obj = JS_NEW_OBJECT();
    JS_SET_PTR(obj, timer)
    JS_SET_NUMBER(obj, "elapsed", 0);

    JS_RETURN(obj);
}

// OP.timer.Update
JS_RETURN_VAL _OPtimerUpdate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    Handle<Object> obj = args[0]->ToObject();

    OPtimer* timer = JS_GET_PTR(obj, OPtimer);
	timer->Tick();

    JS_SET_NUMBER(obj, "elapsed", timer->Elapsed);

    JS_RETURN_NULL
}

// OP.timer.Update
JS_RETURN_VAL _OPtimerSetElapsed(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    Handle<Object> obj = args[0]->ToObject();

    OPtimer* timer = JS_GET_PTR(obj, OPtimer);
    timer->Elapsed = args[1]->IntegerValue();

    JS_SET_INTEGER(obj, "elapsed", (int32_t)timer->Elapsed);

    JS_RETURN_NULL
}

void OPtimerWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> timer = JS_NEW_OBJECT();
    JS_SET_METHOD(timer, "Create", _OPtimerCreate);
    JS_SET_METHOD(timer, "Update", _OPtimerUpdate);
    JS_SET_METHOD(timer, "SetElapsed", _OPtimerSetElapsed);
    JS_SET_NUMBER(timer, "size", sizeof(OPtimer));
    JS_SET_OBJECT(exports, "timer", timer);
}
#endif

// myobject.cc
#include "./Scripting/include/Node/Human/Wrappers.h"
#include "./Scripting/include/Node/Math/Wrappers.h"

#include "./Core/Core.h"

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

void OPtimerWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> timer = NODE_NEW_OBJECT();
    NODE_SET_METHOD(timer, "Create", _OPtimerCreate);
    NODE_SET_METHOD(timer, "Update", _OPtimerUpdate);
    NODE_SET_OBJECT(exports, "timer", timer);
}
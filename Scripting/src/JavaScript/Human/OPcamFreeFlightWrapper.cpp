// myobject.cc
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)



JS_RETURN_VAL _OPcamFreeFlightUpdate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcamFreeFlight* camPtr = JS_GET_ARG_PTR(args, 0, OPcamFreeFlight);
    OPtimer* timerPtr = JS_GET_ARG_PTR(args, 1, OPtimer);
    OPcamFreeFlightUpdate(camPtr, timerPtr);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPcamFreeFlightUpdateSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcamFreeFlight* camPtr = JS_GET_PTR(args.This(), OPcamFreeFlight);
    OPtimer* timerPtr = JS_GET_ARG_PTR(args, 0, OPtimer);
    OPcamFreeFlightUpdate(camPtr, timerPtr);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPcamFreeFlightDestroy(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcamFreeFlight* camPtr = JS_GET_PTR(args.This(), OPcamFreeFlight);
    //OPcamFreeFlightDestroy(camPtr);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPcamFreeFlightSetRotX(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcamFreeFlight* camPtr = JS_GET_ARG_PTR(args, 0, OPcamFreeFlight);
    camPtr->Rotation.x = args[1]->NumberValue();

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPcamFreeFlightSetRotXSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcamFreeFlight* camPtr = JS_GET_PTR(args.This(), OPcamFreeFlight);
    camPtr->Rotation.x = args[0]->NumberValue();

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPcamFreeFlightSetRotY(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcamFreeFlight* camPtr = JS_GET_ARG_PTR(args, 0, OPcamFreeFlight);
    camPtr->Rotation.y = args[1]->NumberValue();

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPcamFreeFlightSetRotYSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcamFreeFlight* camPtr = JS_GET_PTR(args.This(), OPcamFreeFlight);
    camPtr->Rotation.y = args[0]->NumberValue();

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPcamFreeFlightDestroySelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcamFreeFlight* camPtr = JS_GET_ARG_PTR(args, 0, OPcamFreeFlight);
    //OPcamFreeFlightDestroy(camPtr);

    JS_RETURN_NULL;
}

void OPcamFreeFlightWrapperCreate(Handle<Object> result, OPcamFreeFlight* cam) {
    SCOPE_AND_ISOLATE

    JS_SET_PTR(result, cam);
    JS_SET_METHOD(result, "Update", _OPcamFreeFlightUpdateSelf);
    JS_SET_METHOD(result, "Destroy", _OPcamFreeFlightDestroySelf);
    JS_SET_METHOD(result, "SetRotX", _OPcamFreeFlightSetRotXSelf);
    JS_SET_METHOD(result, "SetRotY", _OPcamFreeFlightSetRotYSelf);

    Handle<Object> internalCam = JS_NEW_OBJECT();
    OPcamWrapper(internalCam, &cam->Camera);
    JS_SET_OBJECT(result, "Camera", internalCam);
}

JS_RETURN_VAL _OPcamFreeFlightCreate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPfloat moveSpeed = 1, rotateSpeed = 1, camNear = 0.1, camFar = 100.0;
    OPvec3 position = OPVEC3_ZERO;

    if(args.Length() > 0) {
        moveSpeed = args[0]->NumberValue();
    }
    if(args.Length() > 1) {
        rotateSpeed = args[1]->NumberValue();
    }
    if(args.Length() > 2) {
        OPvec3* pos = JS_GET_PTR(args[2]->ToObject(), OPvec3);
        position = *pos;
    }
    if(args.Length() > 3) {
        camNear = args[3]->NumberValue();
    }
    if(args.Length() > 4) {
        camFar = args[4]->NumberValue();
    }

    OPcamFreeFlight* cam = OPcamFreeCreate(moveSpeed, rotateSpeed, position, camNear, camFar);
    Handle<Object> result = JS_NEW_OBJECT();
    OPcamFreeFlightWrapperCreate(result, cam);

    JS_RETURN(result);
}

void OPcamFreeFlightWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> camFreeFlight = JS_NEW_OBJECT();
    JS_SET_METHOD(camFreeFlight, "Create", _OPcamFreeFlightCreate);
    JS_SET_METHOD(camFreeFlight, "Update", _OPcamFreeFlightUpdate);
    JS_SET_METHOD(camFreeFlight, "SetRotX", _OPcamFreeFlightSetRotX);
    JS_SET_METHOD(camFreeFlight, "SetRotY", _OPcamFreeFlightSetRotY);
    JS_SET_METHOD(camFreeFlight, "Destroy", _OPcamFreeFlightDestroy);
    JS_SET_NUMBER(camFreeFlight, "size", sizeof(OPcamFreeFlight));
    JS_SET_OBJECT(exports, "camFreeFlight", camFreeFlight);

}

#endif

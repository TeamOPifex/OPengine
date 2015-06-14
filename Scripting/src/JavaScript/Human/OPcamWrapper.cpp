#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Human/include/Rendering/OPcam.h"

JS_RETURN_VAL _OPcamUpdateViewSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcam* ptr = JS_GET_PTR(args.This(), OPcam);
    OPcamUpdateView((*ptr));

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPcamUpdateView(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcam* ptr = JS_GET_ARG_PTR(args, 0, OPcam);
    OPcamUpdateView((*ptr));

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPcamUpdateProjSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcam* ptr = JS_GET_PTR(args.This(), OPcam);
    OPcamUpdateProj((*ptr));

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPcamUpdateProj(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcam* ptr = JS_GET_ARG_PTR(args, 0, OPcam);
    OPcamUpdateProj((*ptr));

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPcamSetPosSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcam* ptr = JS_GET_PTR(args.This(), OPcam);
    OPvec3* pos = JS_GET_ARG_PTR(args, 0, OPvec3);
    ptr->_pos = *pos;
    ptr->_viewStale = 1;

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPcamSetPos(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcam* ptr = JS_GET_ARG_PTR(args, 0, OPcam);
    OPvec3* pos = JS_GET_ARG_PTR(args, 1, OPvec3);
    ptr->_pos = *pos;
    ptr->_viewStale = 1;

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPcamSetTargetSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcam* ptr = JS_GET_PTR(args.This(), OPcam);
    OPvec3* pos = JS_GET_ARG_PTR(args, 0, OPvec3);
    ptr->_targ = *pos;
    ptr->_viewStale = 1;

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPcamSetTarget(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcam* ptr = JS_GET_ARG_PTR(args, 0, OPcam);
    OPvec3* pos = JS_GET_ARG_PTR(args, 1, OPvec3);
    ptr->_targ = *pos;
    ptr->_viewStale = 1;

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPcamPersp(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;
    
    OPcam* ptr = (OPcam*)OPallocZero(sizeof(OPcam));

    *ptr = OPcamPersp(
            OPvec3Create(
                    args[0]->NumberValue(),
                    args[1]->NumberValue(),
                    args[2]->NumberValue()
            ),
            OPvec3Create(
                    args[3]->NumberValue(),
                    args[4]->NumberValue(),
                    args[5]->NumberValue()
            ),
            OPvec3Create(
                    args[6]->NumberValue(),
                    args[7]->NumberValue(),
                    args[8]->NumberValue()
            ),
            args[9]->NumberValue(),
            args[10]->NumberValue(),
            args[11]->NumberValue(),
            args[12]->NumberValue()
    );

    Handle<Object> result = JS_NEW_OBJECT();
    OPcamWrapperCreate(result, ptr);

    JS_RETURN(result);
}

void OPcamWrapperCreate(Handle<Object> result, OPcam* ptr) {
    SCOPE_AND_ISOLATE;
    
    JS_SET_PTR(result, ptr);

    JS_SET_METHOD(result, "UpdateView", _OPcamUpdateViewSelf);
    JS_SET_METHOD(result, "UpdateProj", _OPcamUpdateProjSelf); 
    JS_SET_METHOD(result, "SetPos", _OPcamSetPosSelf);
    JS_SET_METHOD(result, "SetTarget", _OPcamSetTargetSelf);
}

JS_RETURN_VAL _OPcamCreate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcam* ptr = (OPcam*)OPallocZero(sizeof(OPcam));
    Handle<Object> result = JS_NEW_OBJECT();
    OPcamWrapperCreate(result, ptr);

    JS_RETURN(result);
}

void OPcamWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Local<FunctionTemplate> tpl = JS_NEW_FUNCTION_TEMPLATE(_OPcamCreate);
    Handle<Object> cam = tpl->GetFunction();
    JS_SET_METHOD(cam, "Persp", _OPcamPersp);
    JS_SET_METHOD(cam, "UpdateView", _OPcamUpdateView);
    JS_SET_METHOD(cam, "UpdateProj", _OPcamUpdateProj);
    JS_SET_METHOD(cam, "SetPos", _OPcamSetPos);
    JS_SET_METHOD(cam, "SetTarget", _OPcamSetTarget);
    JS_SET_NUMBER(cam, "size", sizeof(OPcam));
    JS_SET_OBJECT(exports, "cam", cam);

}

#endif
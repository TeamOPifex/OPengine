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

JS_RETURN_VAL _OPcamPersp(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;
    OPcam* ptr = (OPcam*)OPallocZero(sizeof(OPcam));
    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_PTR(result, ptr);

    //JS_SET_METHOD(result, "Log", _OPvec3LogSelf);

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

    JS_SET_METHOD(result, "UpdateView", _OPcamUpdateViewSelf);
    JS_SET_METHOD(result, "UpdateProj", _OPcamUpdateProjSelf);

    JS_RETURN(result);
}

JS_RETURN_VAL _OPcamCreate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcam* ptr = (OPcam*)OPallocZero(sizeof(OPcam));
    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_PTR(result, ptr);

    JS_SET_METHOD(result, "UpdateView", _OPcamUpdateViewSelf);
    JS_SET_METHOD(result, "UpdateProj", _OPcamUpdateProjSelf);

    JS_RETURN(result);
}

void OPcamWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Local<FunctionTemplate> tpl = JS_NEW_FUNCTION_TEMPLATE(_OPcamCreate);
    Handle<Object> cam = tpl->GetFunction();
    JS_SET_METHOD(cam, "Persp", _OPcamPersp);
    JS_SET_METHOD(cam, "UpdateView", _OPcamUpdateView);
    JS_SET_METHOD(cam, "UpdateProj", _OPcamUpdateProj);
    JS_SET_OBJECT(exports, "cam", cam);

}

#endif
// myobject.cc
#include "./Scripting/include/Node/Human/Wrappers.h"
#include "./Scripting/include/Node/Math/Wrappers.h"


NODE_RETURN_VAL _OPcamUpdateViewSelf(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcam* ptr = NODE_GET_PTR(args.This(), OPcam);
    OPcamUpdateView((*ptr));

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPcamUpdateView(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcam* ptr = NODE_GET_ARG_PTR(args, 0, OPcam);
    OPcamUpdateView((*ptr));

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPcamUpdateProjSelf(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcam* ptr = NODE_GET_PTR(args.This(), OPcam);
    OPcamUpdateProj((*ptr));

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPcamUpdateProj(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcam* ptr = NODE_GET_ARG_PTR(args, 0, OPcam);
    OPcamUpdateProj((*ptr));

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPcamPersp(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;
    OPcam* ptr = (OPcam*)OPallocZero(sizeof(OPcam));
    Handle<Object> result = NODE_NEW_OBJECT();
    NODE_SET_PTR(result, ptr);

    //NODE_SET_METHOD(result, "Log", _OPvec3LogSelf);

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

    NODE_SET_METHOD(result, "UpdateView", _OPcamUpdateViewSelf);
    NODE_SET_METHOD(result, "UpdateProj", _OPcamUpdateProjSelf);

    NODE_RETURN(result);
}

NODE_RETURN_VAL _OPcamCreate(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcam* ptr = (OPcam*)OPallocZero(sizeof(OPcam));
    Handle<Object> result = NODE_NEW_OBJECT();
    NODE_SET_PTR(result, ptr);

    NODE_SET_METHOD(result, "UpdateView", _OPcamUpdateViewSelf);
    NODE_SET_METHOD(result, "UpdateProj", _OPcamUpdateProjSelf);

    NODE_RETURN(result);
}

void OPcamWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Local<FunctionTemplate> tpl = NODE_NEW_FUNCTION_TEMPLATE(_OPcamCreate);
    Handle<Object> cam = tpl->GetFunction();
    NODE_SET_METHOD(cam, "Persp", _OPcamPersp);
    NODE_SET_METHOD(cam, "UpdateView", _OPcamUpdateView);
    NODE_SET_METHOD(cam, "UpdateProj", _OPcamUpdateProj);
    NODE_SET_OBJECT(exports, "cam", cam);

}
// myobject.cc
#include "./Scripting/include/Node/Math/Wrappers.h"

NODE_RETURN_VAL _OPvec3LogSelf(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec3* ptr = NODE_GET_PTR(args.This(), OPvec3);
    String::Utf8Value str(args[0]->ToString());
    OPvec3Log(*str, *ptr);

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPvec3Log(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec3* ptr = NODE_GET_ARG_PTR(args, 1, OPvec3);
    String::Utf8Value str(args[0]->ToString());
    OPvec3Log(*str, *ptr);

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPvec3Create(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec3* ptr = (OPvec3*)OPalloc(sizeof(OPvec3));
    *ptr = OPVEC3_ZERO;
    Handle<Object> result = NODE_NEW_OBJECT();
    NODE_SET_PTR(result, ptr);

    NODE_SET_METHOD(result, "Log", _OPvec3LogSelf);

    NODE_RETURN(result);
}

void OPvec3Wrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Local<FunctionTemplate> tpl = NODE_NEW_FUNCTION_TEMPLATE(_OPvec3Create);
    Handle<Object> vec3 = tpl->GetFunction();
    NODE_SET_METHOD(vec3, "Log", _OPvec3Log);
    NODE_SET_OBJECT(exports, "vec3", vec3);

}

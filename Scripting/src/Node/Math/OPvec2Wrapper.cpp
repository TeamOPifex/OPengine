// myobject.cc
#include "./Scripting/include/Node/Math/Wrappers.h"

NODE_RETURN_VAL _OPvec2LogSelf(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec2* ptr = NODE_GET_PTR(args.This(), OPvec2);
    String::Utf8Value str(args[0]->ToString());
    OPvec2Log(*str, *ptr);

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPvec2Log(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec2* ptr = NODE_GET_ARG_PTR(args, 1, OPvec2);
    String::Utf8Value str(args[0]->ToString());
    OPvec2Log(*str, *ptr);

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPvec2SetSelf(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec2* ptr = NODE_GET_PTR(args.This(), OPvec2);
    ptr->x = args[0]->NumberValue();
    ptr->y = args[1]->NumberValue();

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPvec2Set(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec2* ptr = NODE_GET_ARG_PTR(args, 0, OPvec2);
    ptr->x = args[1]->NumberValue();
    ptr->y = args[2]->NumberValue();


    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPvec2Create(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec2* ptr = (OPvec2*)OPalloc(sizeof(OPvec2));
    *ptr = OPVEC2_ZERO;
    Handle<Object> result = NODE_NEW_OBJECT();
    NODE_SET_PTR(result, ptr);

    NODE_SET_METHOD(result, "Log", _OPvec2LogSelf);
    NODE_SET_METHOD(result, "Set", _OPvec2SetSelf);

    NODE_RETURN(result);
}

void OPvec2Wrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Local<FunctionTemplate> tpl = NODE_NEW_FUNCTION_TEMPLATE(_OPvec2Create);
    Handle<Object> vec2 = tpl->GetFunction();
    NODE_SET_METHOD(vec2, "Log", _OPvec2Log);
    NODE_SET_METHOD(vec2, "Set", _OPvec2Set);
    NODE_SET_OBJECT(exports, "vec2", vec2);

}

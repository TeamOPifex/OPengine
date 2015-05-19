#include "./Scripting/include/Node/OPscriptNodeHelper.h"

#ifdef OPIFEX_OPTION_NODEJS

#include "./Scripting/include/Node/OPscriptNodeWrapperMath.h"
#include "./Scripting/include/Node/Math/OPvec3Wrapper.h"
#include "./Scripting/include/Node/Math/OPmat4Wrapper.h"

#include "./Math/OPmath.h"
#include "./Core/include/OPlog.h"

NODE_RETURN_VAL Method(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE
    NODE_RETURN(NODE_NEW_STRING("world"));
}

NODE_RETURN_VAL _OPvec2Set(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPvec2* vec2 = NODE_GET_ARG_PTR(args, 0, OPvec2);
    vec2->x = args[1]->NumberValue();
    vec2->y = args[2]->NumberValue();
    OPvec2Log("Test", *vec2);

    NODE_RETURN_NULL
}


void OPscriptNodeWrapperMath(Handle<Object> exports) {
    SCOPE_AND_ISOLATE

    { // OP.vec2
        Handle<Object> vec2 = NODE_NEW_OBJECT();
        NODE_SET_METHOD(vec2, "Set", _OPvec2Set);
        NODE_SET_NUMBER(vec2, "size", sizeof(OPvec2));
        NODE_SET_OBJECT(exports, "vec2", vec2);
    }

    { // OP.mat4
        Handle<Object> mat4 = NODE_NEW_OBJECT();
//        NODE_SET_METHOD(mat4, "RotY", _OPmat4RotY);
//        NODE_SET_METHOD(mat4, "Scl", _OPmat4Scl);
        NODE_SET_NUMBER(mat4, "size", sizeof(OPmat4));
        NODE_SET_OBJECT(exports, "mat4", mat4);
    }

    { // OP.allocLinear

    }

    OPvec3Wrapper(exports);
    OPmat4Wrapper(exports);

}

#endif
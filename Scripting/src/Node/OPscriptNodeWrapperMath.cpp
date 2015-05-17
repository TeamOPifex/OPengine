#include "./Scripting/include/Node/OPscriptNodeHelper.h"

#ifdef OPIFEX_OPTION_NODEJS

#include "./Scripting/include/Node/OPscriptNodeWrapperMath.h"
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

//
//OPmat4 _getOPmat4(Handle<Object> obj) {
//
//    Handle<Array> mat = Handle<Array>::Cast(obj);
//    Handle<Array> row0 = Handle<Array>::Cast(mat->Get(0));
//    Handle<Array> row1 = Handle<Array>::Cast(mat->Get(1));
//    Handle<Array> row2 = Handle<Array>::Cast(mat->Get(2));
//    Handle<Array> row3 = Handle<Array>::Cast(mat->Get(3));
//
//    return OPmat4Create(
//        row0->Get(0)->NumberValue(),
//        row0->Get(1)->NumberValue(),
//        row0->Get(2)->NumberValue(),
//        row0->Get(3)->NumberValue(),
//
//        row1->Get(0)->NumberValue(),
//        row1->Get(1)->NumberValue(),
//        row1->Get(2)->NumberValue(),
//        row1->Get(3)->NumberValue(),
//
//        row2->Get(0)->NumberValue(),
//        row2->Get(1)->NumberValue(),
//        row2->Get(2)->NumberValue(),
//        row2->Get(3)->NumberValue(),
//
//        row3->Get(0)->NumberValue(),
//        row3->Get(1)->NumberValue(),
//        row3->Get(2)->NumberValue(),
//        row3->Get(3)->NumberValue()
//    );
//}



void _setOPmat4(Handle<Object> obj, OPmat4* mat4) {
    SCOPE_AND_ISOLATE

    NODE_SET_PTR(obj, mat4);

}

NODE_RETURN_VAL _OPmat4Scl(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

//    OPmat4* mat = (OPmat4*)OPalloc(sizeof(OPmat4));
//    OPfloat sclx, scly, sclz;
//
//    if(args.Length() == 3) {
//        sclx = args[0]->NumberValue();
//        scly = args[1]->NumberValue();
//        sclz = args[2]->NumberValue();
//        *mat = OPmat4Scl(mat, sclx, scly, sclz);
//    } else {
//        *mat = OPmat4Scl(args[0]->NumberValue());
//    }
//
//    Handle<Object> result = NODE_NEW_OBJECT();
//    _setOPmat4(result, mat);
//    NODE_RETURN(result);
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

    OPmat4Wrapper::Init(exports);

}

#endif
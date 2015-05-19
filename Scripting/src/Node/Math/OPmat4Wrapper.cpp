#include "./Scripting/include/Node/Math/Wrappers.h"

#include "./Math/include/OPmat4.h"

NODE_RETURN_VAL _OPmat4ScaleSelf(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4* ptr = NODE_GET_PTR(args.This(), OPmat4);
    if(args.Length() == 3) {
        *ptr = OPmat4Scl(*ptr, args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue());
    } else if(args.Length() == 1) {
        *ptr = OPmat4Scl(*ptr, args[0]->NumberValue());
    }

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPmat4Scale(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4* ptr = NODE_GET_ARG_PTR(args, 0, OPmat4);
    if(args.Length() == 3) {
        *ptr = OPmat4Scl(*ptr, args[1]->NumberValue(), args[2]->NumberValue(), args[3]->NumberValue());
    } else if(args.Length() == 1) {
        *ptr = OPmat4Scl(*ptr, args[1]->NumberValue());
    }

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPmat4TranslateSelf(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4* ptr = NODE_GET_PTR(args.This(), OPmat4);
    *ptr = OPmat4Translate(*ptr, args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue());

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPmat4Translate(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4* ptr = NODE_GET_ARG_PTR(args, 0, OPmat4);
    *ptr = OPmat4Translate(*ptr, args[1]->NumberValue(), args[2]->NumberValue(), args[3]->NumberValue());

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPmat4RotXSelf(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4* ptr = NODE_GET_PTR(args.This(), OPmat4);
    *ptr = OPmat4RotX(*ptr, args[0]->NumberValue());

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPmat4RotX(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4* ptr = NODE_GET_ARG_PTR(args, 0, OPmat4);
    *ptr = OPmat4RotX(*ptr, args[1]->NumberValue());

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPmat4RotYSelf(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4* ptr = NODE_GET_PTR(args.This(), OPmat4);
    *ptr = OPmat4RotY(*ptr, args[0]->NumberValue());

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPmat4RotY(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4* ptr = NODE_GET_ARG_PTR(args, 0, OPmat4);
    *ptr = OPmat4RotY(*ptr, args[1]->NumberValue());

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPmat4RotZSelf(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4* ptr = NODE_GET_PTR(args.This(), OPmat4);
    *ptr = OPmat4RotZ(*ptr, args[0]->NumberValue());

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPmat4RotZ(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4* ptr = NODE_GET_ARG_PTR(args, 0, OPmat4);
    *ptr = OPmat4RotZ(*ptr, args[1]->NumberValue());

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPmat4LogSelf(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4* ptr = NODE_GET_PTR(args.This(), OPmat4);
    String::Utf8Value str(args[0]->ToString());
    OPmat4Log(*str, *ptr);

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPmat4Log(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4* ptr = NODE_GET_ARG_PTR(args, 1, OPmat4);
    String::Utf8Value str(args[0]->ToString());
    OPmat4Log(*str, *ptr);

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPmat4Create(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4* ptr = (OPmat4*)OPalloc(sizeof(OPmat4));
    *ptr = OPMAT4IDENTITY;
    Handle<Object> result = NODE_NEW_OBJECT();
    NODE_SET_PTR(result, ptr);

    NODE_SET_METHOD(result, "RotX", _OPmat4RotXSelf);
    NODE_SET_METHOD(result, "RotY", _OPmat4RotYSelf);
    NODE_SET_METHOD(result, "RotZ", _OPmat4RotZSelf);
    NODE_SET_METHOD(result, "Translate", _OPmat4TranslateSelf);
    NODE_SET_METHOD(result, "Scl", _OPmat4ScaleSelf);
    NODE_SET_METHOD(result, "Log", _OPmat4LogSelf);

    NODE_RETURN(result);
}

void OPmat4Wrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Local<FunctionTemplate> tpl = NODE_NEW_FUNCTION_TEMPLATE(_OPmat4Create);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "Log", _OPmat4Log);

    //constructor.Reset(isolate, tpl->GetFunction());
    Handle<Object> mat4 = tpl->GetFunction();
    NODE_SET_METHOD(mat4, "RotX", _OPmat4RotX);
    NODE_SET_METHOD(mat4, "RotY", _OPmat4RotY);
    NODE_SET_METHOD(mat4, "RotZ", _OPmat4RotZ);
    NODE_SET_METHOD(mat4, "Translate", _OPmat4Translate);
    NODE_SET_METHOD(mat4, "Scl", _OPmat4Scale);
    NODE_SET_METHOD(mat4, "Log", _OPmat4Log);
    NODE_SET_OBJECT(exports, "mat4", mat4);

}

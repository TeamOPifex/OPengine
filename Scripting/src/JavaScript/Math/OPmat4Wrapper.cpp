#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Math/include/OPmat4.h"

JS_RETURN_VAL _OPmat4ScaleSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4* ptr = JS_GET_PTR(args.This(), OPmat4);
    if(args.Length() == 3) {
        *ptr = OPmat4Scl(*ptr, args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue());
    } else if(args.Length() == 1) {
        *ptr = OPmat4Scl(*ptr, args[0]->NumberValue());
    }

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmat4Scale(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4* ptr = JS_GET_ARG_PTR(args, 0, OPmat4);
    if(args.Length() == 3) {
        *ptr = OPmat4Scl(*ptr, args[1]->NumberValue(), args[2]->NumberValue(), args[3]->NumberValue());
    } else if(args.Length() == 1) {
        *ptr = OPmat4Scl(*ptr, args[1]->NumberValue());
    }

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmat4SetScaleSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4* ptr = JS_GET_PTR(args.This(), OPmat4);
    if(args.Length() == 3) {
        *ptr = OPmat4Scl(args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue());
    } else if(args.Length() == 1) {
        *ptr = OPmat4Scl(args[0]->NumberValue());
    }

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmat4SetScale(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4* ptr = JS_GET_ARG_PTR(args, 0, OPmat4);
    if(args.Length() == 3) {
        *ptr = OPmat4Scl(args[1]->NumberValue(), args[2]->NumberValue(), args[3]->NumberValue());
    } else if(args.Length() == 1) {
        *ptr = OPmat4Scl(args[1]->NumberValue());
    }

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmat4TranslateSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4* ptr = JS_GET_PTR(args.This(), OPmat4);
    *ptr = OPmat4Translate(*ptr, args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue());

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmat4Translate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4* ptr = JS_GET_ARG_PTR(args, 0, OPmat4);
    *ptr = OPmat4Translate(*ptr, args[1]->NumberValue(), args[2]->NumberValue(), args[3]->NumberValue());

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmat4SetTranslateSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4* ptr = JS_GET_PTR(args.This(), OPmat4);
    *ptr = OPmat4Translate(args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue());

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmat4SetTranslate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4* ptr = JS_GET_ARG_PTR(args, 0, OPmat4);
    *ptr = OPmat4Translate(args[1]->NumberValue(), args[2]->NumberValue(), args[3]->NumberValue());

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmat4RotXSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4* ptr = JS_GET_PTR(args.This(), OPmat4);
    *ptr = OPmat4RotX(*ptr, args[0]->NumberValue());

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmat4RotX(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4* ptr = JS_GET_ARG_PTR(args, 0, OPmat4);
    *ptr = OPmat4RotX(*ptr, args[1]->NumberValue());

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmat4RotYSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4* ptr = JS_GET_PTR(args.This(), OPmat4);
    *ptr = OPmat4RotY(*ptr, args[0]->NumberValue());

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmat4RotY(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4* ptr = JS_GET_ARG_PTR(args, 0, OPmat4);
    *ptr = OPmat4RotY(*ptr, args[1]->NumberValue());

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmat4RotZSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4* ptr = JS_GET_PTR(args.This(), OPmat4);
    *ptr = OPmat4RotZ(*ptr, args[0]->NumberValue());

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmat4RotZ(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4* ptr = JS_GET_ARG_PTR(args, 0, OPmat4);
    *ptr = OPmat4RotZ(*ptr, args[1]->NumberValue());

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmat4LogSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4* ptr = JS_GET_PTR(args.This(), OPmat4);
    String::Utf8Value str(args[0]->ToString());
    OPmat4Log(*str, *ptr);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmat4Log(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4* ptr = JS_GET_ARG_PTR(args, 1, OPmat4);
    String::Utf8Value str(args[0]->ToString());
    OPmat4Log(*str, *ptr);

    JS_RETURN_NULL;
}

void OPmat4WrapperCreate(Handle<Object> result, OPmat4* ptr) {
    SCOPE_AND_ISOLATE

    JS_SET_PTR(result, ptr);
    JS_SET_METHOD(result, "RotX", _OPmat4RotXSelf);
    JS_SET_METHOD(result, "RotY", _OPmat4RotYSelf);
    JS_SET_METHOD(result, "RotZ", _OPmat4RotZSelf);
    JS_SET_METHOD(result, "Translate", _OPmat4TranslateSelf);
    JS_SET_METHOD(result, "SetTranslate", _OPmat4SetTranslateSelf);
    JS_SET_METHOD(result, "Scl", _OPmat4ScaleSelf);
    JS_SET_METHOD(result, "SetScl", _OPmat4SetScaleSelf);
    JS_SET_METHOD(result, "Log", _OPmat4LogSelf);
}

JS_RETURN_VAL _OPmat4Create(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4* ptr = (OPmat4*)OPalloc(sizeof(OPmat4));
    *ptr = OPMAT4IDENTITY;
    Handle<Object> result = JS_NEW_OBJECT();
    OPmat4WrapperCreate(result, ptr);
    
    JS_RETURN(result);
}

void OPmat4Wrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Local<FunctionTemplate> tpl = JS_NEW_FUNCTION_TEMPLATE(_OPmat4Create);

    // Prototype
    JS_SET_PROTOTYPE_METHOD(tpl, "Log", _OPmat4Log);

    //constructor.Reset(isolate, tpl->GetFunction());
    Handle<Object> mat4 = tpl->GetFunction();
    JS_SET_METHOD(mat4, "RotX", _OPmat4RotX);
    JS_SET_METHOD(mat4, "RotY", _OPmat4RotY);
    JS_SET_METHOD(mat4, "RotZ", _OPmat4RotZ);
    JS_SET_METHOD(mat4, "Translate", _OPmat4Translate);
    JS_SET_METHOD(mat4, "SetTranslate", _OPmat4SetTranslate);
    JS_SET_METHOD(mat4, "Scl", _OPmat4Scale);
    JS_SET_METHOD(mat4, "SetScl", _OPmat4SetScale);
    JS_SET_METHOD(mat4, "Log", _OPmat4Log);
    JS_SET_NUMBER(mat4, "size", sizeof(OPmat4));
    JS_SET_OBJECT(exports, "mat4", mat4);

}

#endif

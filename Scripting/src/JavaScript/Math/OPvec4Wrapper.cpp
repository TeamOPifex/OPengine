#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Math/include/OPvec4.h"

JS_RETURN_VAL _OPvec4LogSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec4* ptr = JS_GET_PTR(args.This(), OPvec4);
    String::Utf8Value str(args[0]->ToString());
    OPvec4Log(*str, *ptr);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPvec4Log(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec4* ptr = JS_GET_ARG_PTR(args, 1, OPvec4);
    String::Utf8Value str(args[0]->ToString());
    OPvec4Log(*str, *ptr);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPvec4SetSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPlog("Setting Vec4 Self");

    OPvec4* ptr = JS_GET_PTR(args.This(), OPvec4);
    ptr->x = args[0]->NumberValue();
    ptr->y = args[1]->NumberValue();
    ptr->z = args[2]->NumberValue();
    ptr->w = args[3]->NumberValue();

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPvec4Set(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPlog("Setting Vec4");

    OPvec4* ptr = JS_GET_ARG_PTR(args, 0, OPvec4);
    ptr->x = args[1]->NumberValue();
    ptr->y = args[2]->NumberValue();
    ptr->z = args[3]->NumberValue();
    ptr->w = args[4]->NumberValue();

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPvec4Destroy(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec4* ptr = JS_GET_ARG_PTR(args, 0, OPvec4);
    OPfree(ptr);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPvec4DestroySelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec4* ptr = JS_GET_PTR(args.This(), OPvec4);
    OPfree(ptr);

    JS_RETURN_NULL;
}

Handle<Object> _OPvec4Setup(Handle<Object> result, OPvec4* ptr){
    SCOPE_AND_ISOLATE;

    JS_SET_PTR(result, ptr);
    JS_SET_METHOD(result, "Log", _OPvec4LogSelf);
    JS_SET_METHOD(result, "Set", _OPvec4SetSelf);
    JS_SET_METHOD(result, "Destroy", _OPvec4DestroySelf);

    return result;
}

JS_RETURN_VAL _OPvec4Create(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec4* ptr = (OPvec4*)OPalloc(sizeof(OPvec4));
    if(args.Length() == 4) {
        *ptr = OPvec4Create(args[0]->NumberValue(),args[1]->NumberValue(),args[2]->NumberValue(),args[3]->NumberValue());
    } else {
        *ptr = OPvec4Create(0,0,0,0);
    }

    JS_RETURN(_OPvec4Setup(JS_NEW_OBJECT(), ptr));
}

void OPvec4Wrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Local<FunctionTemplate> tpl = JS_NEW_FUNCTION_TEMPLATE(_OPvec4Create);
    Handle<Object> vec4 = tpl->GetFunction();
    JS_SET_METHOD(vec4, "Create", _OPvec4Create);
    JS_SET_METHOD(vec4, "Set", _OPvec4Set);
    JS_SET_METHOD(vec4, "Log", _OPvec4Log);
    JS_SET_METHOD(vec4, "Destroy", _OPvec4Destroy);
    JS_SET_NUMBER(vec4, "size", sizeof(OPvec4));
    JS_SET_OBJECT(exports, "vec4", vec4);

}

#endif

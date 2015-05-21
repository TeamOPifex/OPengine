#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Math/include/OPvec3.h"

JS_RETURN_VAL _OPvec3LogSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec3* ptr = JS_GET_PTR(args.This(), OPvec3);
    String::Utf8Value str(args[0]->ToString());
    OPvec3Log(*str, *ptr);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPvec3Log(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec3* ptr = JS_GET_ARG_PTR(args, 1, OPvec3);
    String::Utf8Value str(args[0]->ToString());
    OPvec3Log(*str, *ptr);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPvec3Create(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec3* ptr = (OPvec3*)OPalloc(sizeof(OPvec3));
    *ptr = OPVEC3_ZERO;
    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_PTR(result, ptr);

    JS_SET_METHOD(result, "Log", _OPvec3LogSelf);

    JS_RETURN(result);
}

void OPvec3Wrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Local<FunctionTemplate> tpl = JS_NEW_FUNCTION_TEMPLATE(_OPvec3Create);
    Handle<Object> vec3 = tpl->GetFunction();
    JS_SET_METHOD(vec3, "Log", _OPvec3Log);
    JS_SET_OBJECT(exports, "vec3", vec3);

}

#endif
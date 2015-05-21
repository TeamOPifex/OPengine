#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Math/include/OPvec2.h"

JS_RETURN_VAL _OPvec2LogSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec2* ptr = JS_GET_PTR(args.This(), OPvec2);
    String::Utf8Value str(args[0]->ToString());
    OPvec2Log(*str, *ptr);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPvec2Log(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec2* ptr = JS_GET_ARG_PTR(args, 1, OPvec2);
    String::Utf8Value str(args[0]->ToString());
    OPvec2Log(*str, *ptr);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPvec2SetSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec2* ptr = JS_GET_PTR(args.This(), OPvec2);
    ptr->x = args[0]->NumberValue();
    ptr->y = args[1]->NumberValue();

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPvec2Set(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec2* ptr = JS_GET_ARG_PTR(args, 0, OPvec2);
    ptr->x = args[1]->NumberValue();
    ptr->y = args[2]->NumberValue();


    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPvec2Create(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec2* ptr = (OPvec2*)OPalloc(sizeof(OPvec2));
    *ptr = OPVEC2_ZERO;
    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_PTR(result, ptr);

    JS_SET_METHOD(result, "Log", _OPvec2LogSelf);
    JS_SET_METHOD(result, "Set", _OPvec2SetSelf);

    JS_RETURN(result);
}

void OPvec2Wrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Local<FunctionTemplate> tpl = JS_NEW_FUNCTION_TEMPLATE(_OPvec2Create);
    Handle<Object> vec2 = tpl->GetFunction();
    JS_SET_METHOD(vec2, "Log", _OPvec2Log);
    JS_SET_METHOD(vec2, "Set", _OPvec2Set);
    JS_SET_OBJECT(exports, "vec2", vec2);

}

#endif
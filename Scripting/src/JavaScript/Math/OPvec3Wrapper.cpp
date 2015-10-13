#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Math/include/OPvec3.h"

void OPvec3ResetValues(Handle<Object> result, OPvec3* ptr) {
    SCOPE_AND_ISOLATE

    JS_SET_NUMBER(result, "x", ptr->x);
    JS_SET_NUMBER(result, "y", ptr->y);
    JS_SET_NUMBER(result, "z", ptr->z);
}

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

JS_RETURN_VAL _OPvec3SetSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec3* ptr = JS_GET_PTR(args.This(), OPvec3);
    ptr->x = args[0]->NumberValue();
    ptr->y = args[1]->NumberValue();
    ptr->z = args[2]->NumberValue();
    OPvec3ResetValues(args.This(), ptr);

    JS_RETURN(args.This());
}

JS_RETURN_VAL _OPvec3Set(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec3* ptr = JS_GET_ARG_PTR(args, 0, OPvec3);
    ptr->x = args[1]->NumberValue();
    ptr->y = args[2]->NumberValue();
    ptr->z = args[3]->NumberValue();
    OPvec3ResetValues(args[0]->ToObject(), ptr);

    JS_RETURN(args[0]);
}

JS_RETURN_VAL _OPvec3AddSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    // OPlog("Setting Vec3 Self");

    OPvec3* ptr = JS_GET_PTR(args.This(), OPvec3);
    ptr->x += args[0]->NumberValue();
    ptr->y += args[1]->NumberValue();
    ptr->z += args[2]->NumberValue();
    OPvec3ResetValues(args.This(), ptr);

    JS_RETURN(args.This());
}

JS_RETURN_VAL _OPvec3Add(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec3* ptr = JS_GET_ARG_PTR(args, 0, OPvec3);
    ptr->x += args[1]->NumberValue();
    ptr->y += args[2]->NumberValue();
    ptr->z += args[3]->NumberValue();
    OPvec3ResetValues(args[0]->ToObject(), ptr);

    JS_RETURN(args[0]);;
}

JS_RETURN_VAL _OPvec3SubSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    // OPlog("Setting Vec3 Self");

    OPvec3* ptr = JS_GET_PTR(args.This(), OPvec3);
    ptr->x -= args[0]->NumberValue();
    ptr->y -= args[1]->NumberValue();
    ptr->z -= args[2]->NumberValue();
    OPvec3ResetValues(args.This(), ptr);

    JS_RETURN(args.This());
}

JS_RETURN_VAL _OPvec3Sub(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec3* ptr = JS_GET_ARG_PTR(args, 0, OPvec3);
    ptr->x -= args[1]->NumberValue();
    ptr->y -= args[2]->NumberValue();
    ptr->z -= args[3]->NumberValue();
    OPvec3ResetValues(args[0]->ToObject(), ptr);

    JS_RETURN(args[0]);;
}


JS_RETURN_VAL _OPvec3X(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec3* ptr = JS_GET_ARG_PTR(args, 0, OPvec3);

    JS_RETURN(JS_NEW_NUMBER(ptr->x));
}

JS_RETURN_VAL _OPvec3Y(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec3* ptr = JS_GET_ARG_PTR(args, 0, OPvec3);

    JS_RETURN(JS_NEW_NUMBER(ptr->y));
}

JS_RETURN_VAL _OPvec3Z(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec3* ptr = JS_GET_ARG_PTR(args, 0, OPvec3);

    JS_RETURN(JS_NEW_NUMBER(ptr->z));
}

JS_RETURN_VAL _OPvec3XSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec3* ptr = JS_GET_PTR(args.This(), OPvec3);

    JS_RETURN(JS_NEW_NUMBER(ptr->x));
}

JS_RETURN_VAL _OPvec3YSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec3* ptr = JS_GET_PTR(args.This(), OPvec3);

    JS_RETURN(JS_NEW_NUMBER(ptr->y));
}

JS_RETURN_VAL _OPvec3ZSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec3* ptr = JS_GET_PTR(args.This(), OPvec3);

    JS_RETURN(JS_NEW_NUMBER(ptr->z));
}


JS_RETURN_VAL _OPvec3Sync(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec3* ptr = JS_GET_ARG_PTR(args, 0, OPvec3);

    OPvec3ResetValues(args[0]->ToObject(), ptr);

    JS_RETURN(args[0]);
}

JS_RETURN_VAL _OPvec3SyncSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec3* ptr = JS_GET_PTR(args.This(), OPvec3);
    OPvec3ResetValues(args.This(), ptr);

    JS_RETURN(args.This());
}

JS_RETURN_VAL _OPvec3NormSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec3* ptr = JS_GET_PTR(args.This(), OPvec3);
    *ptr = OPvec3Norm(*ptr);

    OPvec3ResetValues(args.This(), ptr);

    JS_RETURN(args.This());
}

JS_RETURN_VAL _OPvec3Cross(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec3* ptr = JS_GET_ARG_PTR(args, 0, OPvec3);
    OPvec3* ptr2 = JS_GET_ARG_PTR(args, 1, OPvec3);
    OPvec3* ptr3 = JS_GET_ARG_PTR(args, 2, OPvec3);

    *ptr = OPvec3Cross(*ptr2, *ptr3);

    OPvec3ResetValues(args[0]->ToObject(), ptr);

    JS_RETURN(args[0]);
}

JS_RETURN_VAL _OPvec3CrossSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec3* ptr = JS_GET_PTR(args.This(), OPvec3);
    OPvec3* ptr2 = JS_GET_ARG_PTR(args, 0, OPvec3);
    OPvec3* ptr3 = JS_GET_ARG_PTR(args, 1, OPvec3);

    *ptr = OPvec3Cross(*ptr2, *ptr3);

    OPvec3ResetValues(args.This(), ptr);

    JS_RETURN(args.This());
}

JS_RETURN_VAL _OPvec3Destroy(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec3* ptr = JS_GET_ARG_PTR(args, 0, OPvec3);
    OPfree(ptr);

    JS_RETURN(args[0]);
}

JS_RETURN_VAL _OPvec3DestroySelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec3* ptr = JS_GET_PTR(args.This(), OPvec3);
    OPfree(ptr);

    JS_RETURN(args.This());
}

Handle<Object> OPvec3WrapperSetup(Handle<Object> result, OPvec3* ptr) {
    SCOPE_AND_ISOLATE;

    JS_SET_PTR(result, ptr);

    JS_SET_METHOD(result, "Log", _OPvec3LogSelf);
    JS_SET_METHOD(result, "Set", _OPvec3SetSelf);
    JS_SET_METHOD(result, "Add", _OPvec3AddSelf);
    JS_SET_METHOD(result, "Sub", _OPvec3SubSelf);
    JS_SET_METHOD(result, "Subtract", _OPvec3SubSelf);
    JS_SET_METHOD(result, "X", _OPvec3XSelf);
    JS_SET_METHOD(result, "Y", _OPvec3YSelf);
    JS_SET_METHOD(result, "Z", _OPvec3ZSelf);
    JS_SET_METHOD(result, "Norm", _OPvec3NormSelf);
    JS_SET_METHOD(result, "Cross", _OPvec3CrossSelf);
    JS_SET_METHOD(result, "Sync", _OPvec3SyncSelf);
    JS_SET_METHOD(result, "Destroy", _OPvec3DestroySelf);
    OPvec3ResetValues(result, ptr);

    return result;
}

JS_RETURN_VAL _OPvec3Create(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvec3* ptr = (OPvec3*)OPalloc(sizeof(OPvec3));
    *ptr = OPVEC3_ZERO;
    Handle<Object> result = JS_NEW_OBJECT();
    OPvec3WrapperSetup(result, ptr);

    JS_RETURN(result);
}

void OPvec3Wrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Local<FunctionTemplate> tpl = JS_NEW_FUNCTION_TEMPLATE(_OPvec3Create);
    Handle<Object> vec3 = tpl->GetFunction();
    JS_SET_METHOD(vec3, "Create", _OPvec3Create);
    JS_SET_METHOD(vec3, "Log", _OPvec3Log);
    JS_SET_METHOD(vec3, "Set", _OPvec3Set);
    JS_SET_METHOD(vec3, "Add", _OPvec3Add);
    JS_SET_METHOD(vec3, "Sub", _OPvec3Sub);
    JS_SET_METHOD(vec3, "Subtract", _OPvec3Sub);
    JS_SET_METHOD(vec3, "Cross", _OPvec3Cross);
    JS_SET_METHOD(vec3, "X", _OPvec3X);
    JS_SET_METHOD(vec3, "Y", _OPvec3Y);
    JS_SET_METHOD(vec3, "Z", _OPvec3Z);
    JS_SET_METHOD(vec3, "Sync", _OPvec3Sync);
    JS_SET_METHOD(vec3, "Destroy", _OPvec3Destroy);
    JS_SET_NUMBER(vec3, "size", sizeof(OPvec3));
    JS_SET_OBJECT(exports, "vec3", vec3);
}

#endif

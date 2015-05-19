// myobject.cc
#include "./Scripting/include/Node/Math/OPvec3Wrapper.h"

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
//
//using namespace v8;
//
//Persistent<Function> OPvec3Wrapper::constructor;
//
//OPvec3Wrapper::OPvec3Wrapper() {
//
//}
//
//OPvec3Wrapper::~OPvec3Wrapper() {
//
//}
//
//NODE_RETURN_VAL OPvec3Wrapper::Init(Handle<Object> exports) {
//    SCOPE_AND_ISOLATE;
//
//    // Prepare constructor template
//    Local<FunctionTemplate> tpl = NODE_NEW_FUNCTION_TEMPLATE(New);
//    tpl->SetClassName(NODE_NEW_STRING("OPvec3Wrapper"));
//    tpl->InstanceTemplate()->SetInternalFieldCount(1);
//
//    // Prototype
//    //NODE_SET_PROTOTYPE_METHOD(tpl, "RotX", RotX);
//
//    constructor.Reset(isolate, tpl->GetFunction());
//    Handle<Object> vec3 = tpl->GetFunction();
//    //NODE_SET_METHOD(mat4, "RotX", CreateRotX);
//    NODE_SET_OBJECT(exports, "vec3", vec3);
//
//}
//
//NODE_RETURN_VAL OPvec3Wrapper::New(const NODE_ARGS& args) {
//    SCOPE_AND_ISOLATE;
//
//    if (args.IsConstructCall()) {
//        // Invoked as constructor: `new OPmat4Wrapper(...)`
//        OPvec3Wrapper* obj = new OPvec3Wrapper();
//        obj->value_ = OPvec3Create(args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue());
//        obj->Wrap(args.This());
//        NODE_RETURN(args.This());
//    } else {
//        // Invoked as plain function `OPmat4Wrapper(...)`, turn into construct call.
//        const int argc = 1;
//        Local<Value> argv[argc] = { args[0] };
//        Local<Function> cons = NODE_NEW_FUNCTION(constructor);
//        NODE_RETURN(cons->NewInstance(argc, argv));
//    }
//}
//
////
////NODE_RETURN_VAL OPvec3Wrapper::RotX(const NODE_ARGS& args) {
////    SCOPE_AND_ISOLATE;
////    OPmat4Wrapper* obj = ObjectWrap::Unwrap<OPmat4Wrapper>(args.Holder());
////
////    obj->value_ = OPmat4RotX(obj->value_, args[0]->NumberValue());
////}

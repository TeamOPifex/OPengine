// myobject.cc
#include "./Scripting/include/Node/Math/OPmat4Wrapper.h"

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
//    NODE_SET_PROTOTYPE_METHOD(tpl, "RotX", RotX);
//    NODE_SET_PROTOTYPE_METHOD(tpl, "RotY", RotY);
//    NODE_SET_PROTOTYPE_METHOD(tpl, "RotZ", RotZ);
//    NODE_SET_PROTOTYPE_METHOD(tpl, "Translate", Translate);
//    NODE_SET_PROTOTYPE_METHOD(tpl, "Scl", Scale);
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

//    Handle<Object> mat4 = NODE_NEW_OBJECT();
// 	NODE_SET_METHOD(mat4, "Create", _OPmat4Create);
// 	NODE_SET_METHOD(mat4, "Destroy", _OPmat4Destroy);
// 	NODE_SET_METHOD(mat4, "RotX", _OPmat4RotateX);
// 	NODE_SET_METHOD(mat4, "RotY", _OPmat4RotateY);
// 	NODE_SET_METHOD(mat4, "RotZ", _OPmat4RotateZ);
// 	NODE_SET_METHOD(mat4, "Vec3", _OPmat4Vec3);
// 	NODE_SET_METHOD(mat4, "SetRotX", _OPmat4SetRotateX);
// 	NODE_SET_METHOD(mat4, "SetRotY", _OPmat4SetRotateY);
// 	NODE_SET_METHOD(mat4, "SetRotZ", _OPmat4SetRotateZ);
// 	NODE_SET_METHOD(mat4, "SetVec3", _OPmat4SetVec3);
// 	NODE_SET_METHOD(mat4, "Identity", _OPmat4Identity);
// 	NODE_SET_METHOD(mat4, "Mul", _OPmat4Multiply);
//  NODE_SET_OBJECT(exports, "mat4", mat4);

}
//
//using namespace v8;
//
//Persistent<Function> OPmat4Wrapper::constructor;
//
//OPmat4Wrapper::OPmat4Wrapper() {
//
//}
//
//OPmat4Wrapper::~OPmat4Wrapper() {
//
//}
//
//NODE_RETURN_VAL OPmat4Wrapper::Init(Handle<Object> exports) {
//    SCOPE_AND_ISOLATE;
//
//    // Prepare constructor template
//    Local<FunctionTemplate> tpl = NODE_NEW_FUNCTION_TEMPLATE(New);
//    tpl->SetClassName(NODE_NEW_STRING("OPmat4Wrapper"));
//    tpl->InstanceTemplate()->SetInternalFieldCount(1);
//
//    // Prototype
//    NODE_SET_PROTOTYPE_METHOD(tpl, "RotX", RotX);
//    NODE_SET_PROTOTYPE_METHOD(tpl, "RotY", RotY);
//    NODE_SET_PROTOTYPE_METHOD(tpl, "RotZ", RotZ);
//    NODE_SET_PROTOTYPE_METHOD(tpl, "Translate", Translate);
//    NODE_SET_PROTOTYPE_METHOD(tpl, "Scl", Scale);
//    NODE_SET_PROTOTYPE_METHOD(tpl, "Log", Log);
//
//    constructor.Reset(isolate, tpl->GetFunction());
//    Handle<Object> mat4 = tpl->GetFunction();
//    NODE_SET_METHOD(mat4, "RotX", CreateRotX);
//    NODE_SET_METHOD(mat4, "RotY", CreateRotY);
//    NODE_SET_METHOD(mat4, "RotZ", CreateRotZ);
//    NODE_SET_METHOD(mat4, "Translate", CreateTranslate);
//    NODE_SET_METHOD(mat4, "Scl", CreateScale);
//    NODE_SET_OBJECT(exports, "mat4", mat4);
//
//}
//
//NODE_RETURN_VAL OPmat4Wrapper::New(const NODE_ARGS& args) {
//    SCOPE_AND_ISOLATE;
//
//    if (args.IsConstructCall()) {
//        // Invoked as constructor: `new OPmat4Wrapper(...)`
//        OPmat4Wrapper* obj = new OPmat4Wrapper();
//        obj->value_ = OPMAT4IDENTITY;
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
//
//NODE_RETURN_VAL OPmat4Wrapper::RotX(const NODE_ARGS& args) {
//    SCOPE_AND_ISOLATE;
//    OPmat4Wrapper* obj = ObjectWrap::Unwrap<OPmat4Wrapper>(args.Holder());
//
//    obj->value_ = OPmat4RotX(obj->value_, args[0]->NumberValue());
//}
//
//NODE_RETURN_VAL OPmat4Wrapper::CreateRotX(const NODE_ARGS& args) {
//    SCOPE_AND_ISOLATE;
//
//    OPmat4Wrapper* obj;
//    OPint passedObj = args.Length() == 4;
//
//    if(passedObj) {
//        obj = ObjectWrap::Unwrap<OPmat4Wrapper>(args[0]->ToObject());
//        obj->value_ = OPmat4RotX(obj->value_, args[1]->NumberValue());
//
//        NODE_RETURN_NULL;
//    } else {
//        NODE_SETUP_INSTANCE;
//        obj = ObjectWrap::Unwrap<OPmat4Wrapper>(instance);
//        obj->value_ = OPmat4RotX(obj->value_, args[1]->NumberValue());
//
//        NODE_RETURN(instance);
//    }
//}
//
//
//NODE_RETURN_VAL OPmat4Wrapper::RotY(const NODE_ARGS& args) {
//    SCOPE_AND_ISOLATE;
//    OPmat4Wrapper* obj = ObjectWrap::Unwrap<OPmat4Wrapper>(args.Holder());
//
//    obj->value_ = OPmat4RotY(obj->value_, args[0]->NumberValue());
//}
//
//NODE_RETURN_VAL OPmat4Wrapper::CreateRotY(const NODE_ARGS& args) {
//    SCOPE_AND_ISOLATE;
//
//    OPmat4Wrapper* obj;
//    OPint passedObj = args.Length() == 4;
//
//    if(passedObj) {
//        obj = ObjectWrap::Unwrap<OPmat4Wrapper>(args[0]->ToObject());
//        obj->value_ = OPmat4RotY(obj->value_, args[1]->NumberValue());
//
//        NODE_RETURN_NULL;
//    } else {
//        NODE_SETUP_INSTANCE;
//        obj = ObjectWrap::Unwrap<OPmat4Wrapper>(instance);
//        obj->value_ = OPmat4RotY(obj->value_, args[1]->NumberValue());
//
//        NODE_RETURN(instance);
//    }
//}
//
//
//NODE_RETURN_VAL OPmat4Wrapper::RotZ(const NODE_ARGS& args) {
//    SCOPE_AND_ISOLATE;
//    OPmat4Wrapper* obj = ObjectWrap::Unwrap<OPmat4Wrapper>(args.Holder());
//
//    obj->value_ = OPmat4RotZ(obj->value_, args[0]->NumberValue());
//}
//
//NODE_RETURN_VAL OPmat4Wrapper::CreateRotZ(const NODE_ARGS& args) {
//    SCOPE_AND_ISOLATE;
//
//    OPmat4Wrapper* obj;
//    OPint passedObj = args.Length() == 4;
//
//    if(passedObj) {
//        obj = ObjectWrap::Unwrap<OPmat4Wrapper>(args[0]->ToObject());
//        obj->value_ = OPmat4RotZ(obj->value_, args[1]->NumberValue());
//
//        NODE_RETURN_NULL;
//    } else {
//        NODE_SETUP_INSTANCE;
//        obj = ObjectWrap::Unwrap<OPmat4Wrapper>(instance);
//        obj->value_ = OPmat4RotZ(obj->value_, args[1]->NumberValue());
//
//        NODE_RETURN(instance);
//    }
//}
//
//
//NODE_RETURN_VAL OPmat4Wrapper::Translate(const NODE_ARGS& args) {
//    SCOPE_AND_ISOLATE;
//    OPmat4Wrapper* obj = ObjectWrap::Unwrap<OPmat4Wrapper>(args.Holder());
//
//    obj->value_ = OPmat4Translate(obj->value_, args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue());
//}
//
//NODE_RETURN_VAL OPmat4Wrapper::CreateTranslate(const NODE_ARGS& args) {
//    SCOPE_AND_ISOLATE;
//
//    OPmat4Wrapper* obj;
//    OPint passedObj = args.Length() == 4;
//
//    if(passedObj) {
//        obj = ObjectWrap::Unwrap<OPmat4Wrapper>(args[0]->ToObject());
//        obj->value_ = OPmat4Translate(obj->value_, args[1]->NumberValue(), args[2]->NumberValue(), args[3]->NumberValue());
//
//        NODE_RETURN_NULL;
//    } else {
//        NODE_SETUP_INSTANCE;
//        obj = ObjectWrap::Unwrap<OPmat4Wrapper>(instance);
//        obj->value_ = OPmat4Translate(obj->value_, args[1]->NumberValue(), args[2]->NumberValue(), args[3]->NumberValue());
//
//        NODE_RETURN(instance);
//    }
//}
//
//
//NODE_RETURN_VAL OPmat4Wrapper::Scale(const NODE_ARGS& args) {
//    SCOPE_AND_ISOLATE;
//    OPmat4Wrapper* obj = ObjectWrap::Unwrap<OPmat4Wrapper>(args.Holder());
//
//    if(args.Length() == 1) {
//        obj->value_ = OPmat4Scl(obj->value_, args[0]->NumberValue());
//    } else {
//        obj->value_ = OPmat4Scl(obj->value_, args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue());
//    }
//}
//
//NODE_RETURN_VAL OPmat4Wrapper::CreateScale(const NODE_ARGS& args) {
//    SCOPE_AND_ISOLATE;
//
//    OPmat4Wrapper* obj;
//    OPint passedObj = args.Length() == 2 || args.Length() == 4;
//
//    if(passedObj) {
//        obj = ObjectWrap::Unwrap<OPmat4Wrapper>(args[0]->ToObject());
//
//        if(args.Length() == 2) {
//            obj->value_ = OPmat4Scl(obj->value_, args[1]->NumberValue());
//        } else {
//            obj->value_ = OPmat4Scl(obj->value_, args[1]->NumberValue(), args[2]->NumberValue(), args[3]->NumberValue());
//        }
//
//        NODE_RETURN_NULL;
//    } else {
//        NODE_SETUP_INSTANCE;
//        obj = ObjectWrap::Unwrap<OPmat4Wrapper>(instance);
//
//        if(args.Length() == 1) {
//            obj->value_ = OPmat4Scl(obj->value_, args[1]->NumberValue());
//        } else {
//            obj->value_ = OPmat4Scl(obj->value_, args[1]->NumberValue(), args[2]->NumberValue(), args[3]->NumberValue());
//        }
//
//        NODE_RETURN(instance);
//    }
//}
//

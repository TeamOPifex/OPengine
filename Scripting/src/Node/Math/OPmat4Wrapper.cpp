// myobject.cc
#include "./Scripting/include/Node/Math/OPmat4Wrapper.h"

using namespace v8;

Persistent<Function> OPmat4Wrapper::constructor;

OPmat4Wrapper::OPmat4Wrapper() {

}

OPmat4Wrapper::~OPmat4Wrapper() {

}

NODE_RETURN_VAL OPmat4Wrapper::Init(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    // Prepare constructor template
    Local<FunctionTemplate> tpl = NODE_NEW_FUNCTION_TEMPLATE(New);
    tpl->SetClassName(NODE_NEW_STRING("OPmat4Wrapper"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "RotX", RotX);
    NODE_SET_PROTOTYPE_METHOD(tpl, "RotY", RotY);
    NODE_SET_PROTOTYPE_METHOD(tpl, "RotZ", RotZ);
    NODE_SET_PROTOTYPE_METHOD(tpl, "Translate", Translate);
    NODE_SET_PROTOTYPE_METHOD(tpl, "Scl", Scale);
    NODE_SET_PROTOTYPE_METHOD(tpl, "Log", Log);

    constructor.Reset(isolate, tpl->GetFunction());
    Handle<Object> mat4 = tpl->GetFunction();
    NODE_SET_METHOD(mat4, "RotX", CreateRotX);
    NODE_SET_METHOD(mat4, "RotY", CreateRotY);
    NODE_SET_METHOD(mat4, "RotZ", CreateRotZ);
    NODE_SET_METHOD(mat4, "Translate", CreateTranslate);
    NODE_SET_METHOD(mat4, "Scl", CreateScale);
    NODE_SET_OBJECT(exports, "mat4", mat4);

}

NODE_RETURN_VAL OPmat4Wrapper::New(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    if (args.IsConstructCall()) {
        // Invoked as constructor: `new OPmat4Wrapper(...)`
        OPmat4Wrapper* obj = new OPmat4Wrapper();
        obj->value_ = OPMAT4IDENTITY;
        obj->Wrap(args.This());
        NODE_RETURN(args.This());
    } else {
        // Invoked as plain function `OPmat4Wrapper(...)`, turn into construct call.
        const int argc = 1;
        Local<Value> argv[argc] = { args[0] };
        Local<Function> cons = NODE_NEW_FUNCTION(constructor);
        NODE_RETURN(cons->NewInstance(argc, argv));
    }
}


NODE_RETURN_VAL OPmat4Wrapper::RotX(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;
    OPmat4Wrapper* obj = ObjectWrap::Unwrap<OPmat4Wrapper>(args.Holder());

    obj->value_ = OPmat4RotX(obj->value_, args[0]->NumberValue());
}

NODE_RETURN_VAL OPmat4Wrapper::CreateRotX(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4Wrapper* obj;
    OPint passedObj = args.Length() == 4;

    if(passedObj) {
        obj = ObjectWrap::Unwrap<OPmat4Wrapper>(args[0]->ToObject());
        obj->value_ = OPmat4RotX(obj->value_, args[1]->NumberValue());

        NODE_RETURN_NULL;
    } else {
        NODE_SETUP_INSTANCE;
        obj = ObjectWrap::Unwrap<OPmat4Wrapper>(instance);
        obj->value_ = OPmat4RotX(obj->value_, args[1]->NumberValue());

        NODE_RETURN(instance);
    }
}


NODE_RETURN_VAL OPmat4Wrapper::RotY(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;
    OPmat4Wrapper* obj = ObjectWrap::Unwrap<OPmat4Wrapper>(args.Holder());

    obj->value_ = OPmat4RotY(obj->value_, args[0]->NumberValue());
}

NODE_RETURN_VAL OPmat4Wrapper::CreateRotY(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4Wrapper* obj;
    OPint passedObj = args.Length() == 4;

    if(passedObj) {
        obj = ObjectWrap::Unwrap<OPmat4Wrapper>(args[0]->ToObject());
        obj->value_ = OPmat4RotY(obj->value_, args[1]->NumberValue());

        NODE_RETURN_NULL;
    } else {
        NODE_SETUP_INSTANCE;
        obj = ObjectWrap::Unwrap<OPmat4Wrapper>(instance);
        obj->value_ = OPmat4RotY(obj->value_, args[1]->NumberValue());

        NODE_RETURN(instance);
    }
}


NODE_RETURN_VAL OPmat4Wrapper::RotZ(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;
    OPmat4Wrapper* obj = ObjectWrap::Unwrap<OPmat4Wrapper>(args.Holder());

    obj->value_ = OPmat4RotZ(obj->value_, args[0]->NumberValue());
}

NODE_RETURN_VAL OPmat4Wrapper::CreateRotZ(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4Wrapper* obj;
    OPint passedObj = args.Length() == 4;

    if(passedObj) {
        obj = ObjectWrap::Unwrap<OPmat4Wrapper>(args[0]->ToObject());
        obj->value_ = OPmat4RotZ(obj->value_, args[1]->NumberValue());

        NODE_RETURN_NULL;
    } else {
        NODE_SETUP_INSTANCE;
        obj = ObjectWrap::Unwrap<OPmat4Wrapper>(instance);
        obj->value_ = OPmat4RotZ(obj->value_, args[1]->NumberValue());

        NODE_RETURN(instance);
    }
}


NODE_RETURN_VAL OPmat4Wrapper::Translate(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;
    OPmat4Wrapper* obj = ObjectWrap::Unwrap<OPmat4Wrapper>(args.Holder());

    obj->value_ = OPmat4Translate(obj->value_, args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue());
}

NODE_RETURN_VAL OPmat4Wrapper::CreateTranslate(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4Wrapper* obj;
    OPint passedObj = args.Length() == 4;

    if(passedObj) {
        obj = ObjectWrap::Unwrap<OPmat4Wrapper>(args[0]->ToObject());
        obj->value_ = OPmat4Translate(obj->value_, args[1]->NumberValue(), args[2]->NumberValue(), args[3]->NumberValue());

        NODE_RETURN_NULL;
    } else {
        NODE_SETUP_INSTANCE;
        obj = ObjectWrap::Unwrap<OPmat4Wrapper>(instance);
        obj->value_ = OPmat4Translate(obj->value_, args[1]->NumberValue(), args[2]->NumberValue(), args[3]->NumberValue());

        NODE_RETURN(instance);
    }
}


NODE_RETURN_VAL OPmat4Wrapper::Scale(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;
    OPmat4Wrapper* obj = ObjectWrap::Unwrap<OPmat4Wrapper>(args.Holder());

    if(args.Length() == 1) {
        obj->value_ = OPmat4Scl(obj->value_, args[0]->NumberValue());
    } else {
        obj->value_ = OPmat4Scl(obj->value_, args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue());
    }
}

NODE_RETURN_VAL OPmat4Wrapper::CreateScale(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmat4Wrapper* obj;
    OPint passedObj = args.Length() == 2 || args.Length() == 4;

    if(passedObj) {
        obj = ObjectWrap::Unwrap<OPmat4Wrapper>(args[0]->ToObject());

        if(args.Length() == 2) {
            obj->value_ = OPmat4Scl(obj->value_, args[1]->NumberValue());
        } else {
            obj->value_ = OPmat4Scl(obj->value_, args[1]->NumberValue(), args[2]->NumberValue(), args[3]->NumberValue());
        }

        NODE_RETURN_NULL;
    } else {
        NODE_SETUP_INSTANCE;
        obj = ObjectWrap::Unwrap<OPmat4Wrapper>(instance);

        if(args.Length() == 1) {
            obj->value_ = OPmat4Scl(obj->value_, args[1]->NumberValue());
        } else {
            obj->value_ = OPmat4Scl(obj->value_, args[1]->NumberValue(), args[2]->NumberValue(), args[3]->NumberValue());
        }

        NODE_RETURN(instance);
    }
}


NODE_RETURN_VAL OPmat4Wrapper::Log(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;
    OPmat4Wrapper* obj = ObjectWrap::Unwrap<OPmat4Wrapper>(args.Holder());

    if(args.Length() == 0) {
        OPmat4Log("OPmat4", obj->value_);
    } else {
        String::Utf8Value str(args[0]->ToString());
        OPmat4Log(*str, obj->value_);
    }
}
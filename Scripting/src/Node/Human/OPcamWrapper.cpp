// myobject.cc
#include "./Scripting/include/Node/Human/OPcamWrapper.h"

using namespace v8;

Persistent<Function> OPcamWrapper::constructorPersp;
Persistent<Function> OPcamWrapper::constructorOrtho;

OPcamWrapper::OPcamWrapper() {

}

OPcamWrapper::~OPcamWrapper() {

}

NODE_RETURN_VAL OPcamWrapper::Init(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    // Prepare constructor template
    Local<FunctionTemplate> tplPersp = NODE_NEW_FUNCTION_TEMPLATE(NewPersp);
    Local<FunctionTemplate> tplOrtho = NODE_NEW_FUNCTION_TEMPLATE(NewOrtho);

    tplPersp->SetClassName(NODE_NEW_STRING("OPcamWrapper"));
    tplOrtho->SetClassName(NODE_NEW_STRING("OPcamWrapper"));

    tplPersp->InstanceTemplate()->SetInternalFieldCount(1);
    tplOrtho->InstanceTemplate()->SetInternalFieldCount(1);

    NODE_SET_PROTOTYPE_METHOD(tplPersp, "UpdateView", UpdateView);
    NODE_SET_PROTOTYPE_METHOD(tplPersp, "UpdateProj", UpdateProj);

    NODE_SET_PROTOTYPE_METHOD(tplOrtho, "UpdateView", UpdateView);
    NODE_SET_PROTOTYPE_METHOD(tplOrtho, "UpdateProj", UpdateProj);


    constructorPersp.Reset(isolate, tplPersp->GetFunction());
    constructorOrtho.Reset(isolate, tplOrtho->GetFunction());

    Handle<Object> camPersp = tplPersp->GetFunction();
    Handle<Object> camOrtho = tplOrtho->GetFunction();

    Handle<Object> cam = NODE_NEW_OBJECT();
    NODE_SET_OBJECT(cam, "Persp", camPersp);
    NODE_SET_OBJECT(cam, "Ortho", camOrtho);
    NODE_SET_METHOD(cam, "UpdateView", _UpdateView);
    NODE_SET_METHOD(cam, "UpdateProj", _UpdateProj);
    NODE_SET_OBJECT(exports, "cam", cam);

}

NODE_RETURN_VAL OPcamWrapper::NewPersp(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    if (args.IsConstructCall()) {
        // Invoked as constructor: `new OPcamWrapper(...)`
        OPcamWrapper* obj = new OPcamWrapper();
        obj->value_ = OPcamPersp(
                OPvec3Create(
                        args[0]->NumberValue(),
                        args[1]->NumberValue(),
                        args[2]->NumberValue()
                ),
                OPvec3Create(
                        args[3]->NumberValue(),
                        args[4]->NumberValue(),
                        args[5]->NumberValue()
                ),
                OPvec3Create(
                        args[6]->NumberValue(),
                        args[7]->NumberValue(),
                        args[8]->NumberValue()
                ),
                args[9]->NumberValue(),
                args[10]->NumberValue(),
                args[11]->NumberValue(),
                args[12]->NumberValue()
        );
        obj->Wrap(args.This());
        NODE_RETURN(args.This());
    } else {
        // Invoked as plain function `OPcamWrapper(...)`, turn into construct call.
        const int argc = 1;
        Local<Value> argv[argc] = { args[0] };
        Local<Function> cons = NODE_NEW_FUNCTION(constructorPersp);
        NODE_RETURN(cons->NewInstance(argc, argv));
    }
}

NODE_RETURN_VAL OPcamWrapper::NewOrtho(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    if (args.IsConstructCall()) {
        // Invoked as constructor: `new OPcamWrapper(...)`
        OPcamWrapper* obj = new OPcamWrapper();
        obj->value_ = OPcamOrtho(
                OPvec3Create(
                        args[0]->NumberValue(),
                        args[1]->NumberValue(),
                        args[2]->NumberValue()
                ),
                OPvec3Create(
                        args[3]->NumberValue(),
                        args[4]->NumberValue(),
                        args[5]->NumberValue()
                ),
                OPvec3Create(
                        args[6]->NumberValue(),
                        args[7]->NumberValue(),
                        args[8]->NumberValue()
                ),
                args[9]->NumberValue(),
                args[10]->NumberValue(),
                args[11]->NumberValue(),
                args[12]->NumberValue(),
                args[13]->NumberValue(),
                args[14]->NumberValue()
        );

        obj->Wrap(args.This());
        NODE_RETURN(args.This());
    } else {
        // Invoked as plain function `OPcamWrapper(...)`, turn into construct call.
        const int argc = 1;
        Local<Value> argv[argc] = { args[0] };
        Local<Function> cons = NODE_NEW_FUNCTION(constructorOrtho);
        NODE_RETURN(cons->NewInstance(argc, argv));
    }
}


NODE_RETURN_VAL OPcamWrapper::UpdateView(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;
    OPcamWrapper* obj = ObjectWrap::Unwrap<OPcamWrapper>(args.Holder());
    OPcamUpdateView(obj->value_);
}

NODE_RETURN_VAL OPcamWrapper::_UpdateView(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcamWrapper* obj;
    OPint passedObj = args.Length() == 1;

    if(passedObj) {
        obj = ObjectWrap::Unwrap<OPcamWrapper>(args[0]->ToObject());
        OPcamUpdateView(obj->value_);

        NODE_RETURN_NULL;
    }
}

NODE_RETURN_VAL OPcamWrapper::UpdateProj(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;
    OPcamWrapper* obj = ObjectWrap::Unwrap<OPcamWrapper>(args.Holder());
    OPcamUpdateProj(obj->value_);
}

NODE_RETURN_VAL OPcamWrapper::_UpdateProj(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcamWrapper* obj;
    OPint passedObj = args.Length() == 1;

    if(passedObj) {
        obj = ObjectWrap::Unwrap<OPcamWrapper>(args[0]->ToObject());
        OPcamUpdateProj(obj->value_);

        NODE_RETURN_NULL;
    }
}
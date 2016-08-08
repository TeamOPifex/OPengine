#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Human/include/Rendering/OPcam.h"

JS_RETURN_VAL _OPcamUpdateSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcam* ptr = JS_GET_PTR(args.This(), OPcam);
	ptr->Update();

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPcamUpdate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcam* ptr = JS_GET_ARG_PTR(args, 0, OPcam);
	ptr->Update();

    JS_RETURN_NULL;
}


JS_RETURN_VAL _OPcamGetViewSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcam* ptr = JS_GET_PTR(args.This(), OPcam);
    OPmat4* result = JS_GET_ARG_PTR(args, 0, OPmat4);
    *result = ptr->view;

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPcamGetView(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcam* ptr = JS_GET_ARG_PTR(args, 0, OPcam);
    OPmat4* result = JS_GET_ARG_PTR(args, 1, OPmat4);
    *result = ptr->view;

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPcamGetProjSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcam* ptr = JS_GET_PTR(args.This(), OPcam);
    OPmat4* result = JS_GET_ARG_PTR(args, 0, OPmat4);
    *result = ptr->proj;

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPcamGetProj(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcam* ptr = JS_GET_ARG_PTR(args, 0, OPcam);
    OPmat4* result = JS_GET_ARG_PTR(args, 1, OPmat4);
    *result = ptr->proj;

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPcamSetPosSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    ASSERT(args.Length() > 0 &&
            (args[0]->IsArray() || args.Length() >= 3),
            "OPcamSetPos requires an array or 3 floats"
        );

    OPcam* ptr = JS_GET_PTR(args.This(), OPcam);

    if(args[0]->IsArray()) {
        Handle<Array> arr = Handle<Array>::Cast(args[0]);
        ptr->pos.x = arr->Get(0)->NumberValue();
        ptr->pos.y = arr->Get(1)->NumberValue();
        ptr->pos.z = arr->Get(2)->NumberValue();
    } else {
        ptr->pos.x = args[0]->NumberValue();
        ptr->pos.y = args[1]->NumberValue();
        ptr->pos.z = args[2]->NumberValue();
    }
	ptr->Update();

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPcamSetPos(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    ASSERT(args.Length() > 1 &&
            (args[1]->IsArray() || args.Length() >= 4),
            "OPcamSetPos requires an array or 3 floats"
        );

    OPcam* ptr = JS_GET_ARG_PTR(args, 0, OPcam);
    if(args[1]->IsArray()) {
        Handle<Array> arr = Handle<Array>::Cast(args[1]);
        ptr->pos.x = arr->Get(0)->NumberValue();
        ptr->pos.y = arr->Get(1)->NumberValue();
        ptr->pos.z = arr->Get(2)->NumberValue();
    } else {
        ptr->pos.x = args[1]->NumberValue();
        ptr->pos.y = args[2]->NumberValue();
        ptr->pos.z = args[3]->NumberValue();
    }
	ptr->Update();

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPcamSetTargetSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcam* ptr = JS_GET_PTR(args.This(), OPcam);
    ptr->target.x = args[0]->NumberValue();
    ptr->target.y = args[1]->NumberValue();
    ptr->target.z = args[2]->NumberValue();
	ptr->Update();

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPcamSetTarget(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcam* ptr = JS_GET_ARG_PTR(args, 0, OPcam);
    ptr->target.x = args[1]->NumberValue();
    ptr->target.y = args[2]->NumberValue();
    ptr->target.z = args[3]->NumberValue();
	ptr->Update();

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPcamGetPosSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcam* ptr = JS_GET_PTR(args.This(), OPcam);
    Handle<Array> result = JS_NEW_ARRAY();
    result->Set(0, JS_NEW_NUMBER(ptr->pos.x));
    result->Set(1, JS_NEW_NUMBER(ptr->pos.y));
    result->Set(2, JS_NEW_NUMBER(ptr->pos.z));

    JS_RETURN(result);
}

JS_RETURN_VAL _OPcamGetPos(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcam* ptr = JS_GET_ARG_PTR(args, 0, OPcam);
    Handle<Array> result = JS_NEW_ARRAY();
    result->Set(0, JS_NEW_NUMBER(ptr->pos.x));
    result->Set(1, JS_NEW_NUMBER(ptr->pos.y));
    result->Set(2, JS_NEW_NUMBER(ptr->pos.z));

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPcamSetPerspective(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcam* ptr = JS_GET_ARG_PTR(args, 0, OPcam);
    ptr->SetPerspective(
        OPvec3Create(
            args[1]->NumberValue(),
            args[2]->NumberValue(),
            args[3]->NumberValue()
        ), OPvec3Create(
            args[4]->NumberValue(),
            args[5]->NumberValue(),
            args[6]->NumberValue()
        ));

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPcamPersp(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcam* ptr = (OPcam*)OPallocZero(sizeof(OPcam));

    ptr->SetPerspective(
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

    Handle<Object> result = JS_NEW_OBJECT();
    OPcamWrapper(result, ptr);

    JS_RETURN(result);
}

JS_RETURN_VAL _OPcamOrtho(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcam* ptr = (OPcam*)OPallocZero(sizeof(OPcam));

    ptr->SetOrtho(
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
            args[11]->IntegerValue(),
            args[12]->IntegerValue(),
            args[13]->IntegerValue(),
            args[14]->IntegerValue()
    );

    Handle<Object> result = JS_NEW_OBJECT();
    OPcamWrapper(result, ptr);

    JS_RETURN(result);
}

Handle<Object> OPcamWrapper(Handle<Object> result, OPcam* ptr) {
    SCOPE_AND_ISOLATE;

    JS_SET_PTR(result, ptr);

    JS_SET_METHOD(result, "Update", _OPcamUpdateSelf);
    JS_SET_METHOD(result, "GetView", _OPcamGetViewSelf);
    JS_SET_METHOD(result, "GetProj", _OPcamGetProjSelf);
    JS_SET_METHOD(result, "SetPos", _OPcamSetPosSelf);
    JS_SET_METHOD(result, "GetPos", _OPcamGetPosSelf);
    JS_SET_METHOD(result, "SetTarget", _OPcamSetTargetSelf);

    return result;
}

JS_RETURN_VAL _OPcamCreate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcam* ptr = (OPcam*)OPallocZero(sizeof(OPcam));
    Handle<Object> result = JS_NEW_OBJECT();
    OPcamWrapper(result, ptr);

    JS_RETURN(result);
}

void OPcamWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Local<FunctionTemplate> tpl = JS_NEW_FUNCTION_TEMPLATE(_OPcamCreate);
    Handle<Object> cam = tpl->GetFunction();
    JS_SET_METHOD(cam, "Persp", _OPcamPersp);
    JS_SET_METHOD(cam, "SetPerspective", _OPcamSetPerspective);
    JS_SET_METHOD(cam, "Ortho", _OPcamOrtho);
    JS_SET_METHOD(cam, "Update", _OPcamUpdate);
    JS_SET_METHOD(cam, "GetView", _OPcamGetView);
    JS_SET_METHOD(cam, "GetProj", _OPcamGetProj);
    JS_SET_METHOD(cam, "SetPos", _OPcamSetPos);
    JS_SET_METHOD(cam, "GetPos", _OPcamGetPos);
    JS_SET_METHOD(cam, "SetTarget", _OPcamSetTarget);
    JS_SET_NUMBER(cam, "size", sizeof(OPcam));
    JS_SET_OBJECT(exports, "cam", cam);

}

#endif

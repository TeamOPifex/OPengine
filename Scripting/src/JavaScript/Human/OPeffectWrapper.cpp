// myobject.cc
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Human/Human.h"


JS_RETURN_VAL _OPeffectGen(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPeffect* effect = (OPeffect*)OPalloc(sizeof(OPeffect));

    String::Utf8Value vert(args[0]->ToString());
    String::Utf8Value frag(args[1]->ToString());
    ui32 attrs = args[2]->IntegerValue();
    String::Utf8Value name(args[3]->ToString());
    ui32 vertSize = args[4]->IntegerValue();
    *effect = OPeffectGen(
            *vert,
            *frag,
            attrs,
            *name,
            vertSize
    );

    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_PTR(result, effect);

    JS_RETURN(result);
}

JS_RETURN_VAL _OPeffectBind(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPeffect* effect = JS_GET_ARG_PTR(args, 0, OPeffect);
    OPeffectBind(effect);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPeffectParamMat4(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;


    String::Utf8Value param(args[0]->ToString());
    OPmat4* obj = JS_GET_ARG_PTR(args, 1, OPmat4);
    OPeffectParamMat4(*param, obj);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPeffectParamVec3(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;


    String::Utf8Value param(args[0]->ToString());
    OPvec3* obj = JS_GET_ARG_PTR(args, 1, OPvec3);
    OPeffectParamVec3(*param, obj);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPeffectParamTexture(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    String::Utf8Value param(args[0]->ToString());
    OPtexture* ptr = JS_GET_ARG_PTR(args, 1, OPtexture);
    OPeffectParam(*param, ptr);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPeffectParamCam(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcam* obj = JS_GET_ARG_PTR(args, 0, OPcam);
    OPeffectParam(*obj);

    JS_RETURN_NULL;
}


void OPeffectWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> effect = JS_NEW_OBJECT();
    JS_SET_METHOD(effect, "Gen", _OPeffectGen);
    JS_SET_METHOD(effect, "Bind", _OPeffectBind);
    JS_SET_METHOD(effect, "ParamMat4", _OPeffectParamMat4);
    JS_SET_METHOD(effect, "ParamVec3", _OPeffectParamVec3);
    JS_SET_METHOD(effect, "ParamTex", _OPeffectParamTexture);
    JS_SET_METHOD(effect, "ParamCam", _OPeffectParamCam);
    JS_SET_NUMBER(effect, "size", sizeof(OPeffect));
    JS_SET_OBJECT(exports, "effect", effect);

}

#endif
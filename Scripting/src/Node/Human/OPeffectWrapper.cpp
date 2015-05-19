// myobject.cc
#include "./Scripting/include/Node/Human/Wrappers.h"
#include "./Scripting/include/Node/Math/Wrappers.h"

#include "./Human/Human.h"


NODE_RETURN_VAL _OPeffectGen(const NODE_ARGS& args) {
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

    Handle<Object> result = NODE_NEW_OBJECT();
    NODE_SET_PTR(result, effect);

    NODE_RETURN(result);
}

NODE_RETURN_VAL _OPeffectBind(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPeffect* effect = NODE_GET_ARG_PTR(args, 0, OPeffect);
    OPeffectBind(effect);

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPeffectParamMat4(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;


    String::Utf8Value param(args[0]->ToString());
    OPmat4* obj = NODE_GET_ARG_PTR(args, 1, OPmat4);
    OPeffectParamMat4(*param, obj);

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPeffectParamVec3(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;


    String::Utf8Value param(args[0]->ToString());
    OPvec3* obj = NODE_GET_ARG_PTR(args, 1, OPvec3);
    OPeffectParamVec3(*param, obj);

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPeffectParamTexture(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    String::Utf8Value param(args[0]->ToString());
    OPtexture* ptr = NODE_GET_ARG_PTR(args, 1, OPtexture);
    OPeffectParam(*param, ptr);

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPeffectParamCam(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPcam* obj = NODE_GET_ARG_PTR(args, 0, OPcam);
    OPeffectParam(*obj);

    NODE_RETURN_NULL;
}


void OPeffectWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> effect = NODE_NEW_OBJECT();
    NODE_SET_METHOD(effect, "Gen", _OPeffectGen);
    NODE_SET_METHOD(effect, "Bind", _OPeffectBind);
    NODE_SET_METHOD(effect, "ParamMat4", _OPeffectParamMat4);
    NODE_SET_METHOD(effect, "ParamVec3", _OPeffectParamVec3);
    NODE_SET_METHOD(effect, "ParamTex", _OPeffectParamTexture);
    NODE_SET_METHOD(effect, "ParamCam", _OPeffectParamCam);
    NODE_SET_OBJECT(exports, "effect", effect);

}
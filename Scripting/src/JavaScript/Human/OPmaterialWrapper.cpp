// myobject.cc
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Human/Human.h"


JS_RETURN_VAL _OPmaterialClearParams(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmaterial* material = JS_GET_ARG_PTR(args, 0, OPmaterial);
    OPmaterialClearParams(material);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmaterialClearParamsSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmaterial* material = JS_GET_PTR(args.This(), OPmaterial);
    OPmaterialClearParams(material);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmaterialInit(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmaterial* material = JS_GET_ARG_PTR(args, 0, OPmaterial);
    OPeffect* effect = JS_GET_ARG_PTR(args, 1, OPeffect);
    OPmaterialInit(material, effect);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmaterialAddParam(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmaterial* material = JS_GET_ARG_PTR(args, 0, OPmaterial);

    enum OPmaterialParamType matParamType = (enum OPmaterialParamType)(OPint)args[1]->NumberValue();

    String::Utf8Value str(args[2]->ToString());

    void* data = (void*)(OPint)args[3]->NumberValue();

    ui8 count = (ui8)args[4]->NumberValue();

    OPmaterialAddParam(material, matParamType, *str, data, count);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmaterialAddParamSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmaterial* material = JS_GET_PTR(args.This(), OPmaterial);

    enum OPmaterialParamType matParamType = (enum OPmaterialParamType)(OPint)args[0]->NumberValue();

    String::Utf8Value str(args[1]->ToString());

    void* data = JS_GET_ARG_PTR(args, 2, void);

    ui8 count = (ui8)args[3]->NumberValue();

    OPmaterialAddParam(material, matParamType, *str, data, count);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmaterialBind(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmaterial* material = JS_GET_ARG_PTR(args, 0, OPmaterial);
    OPmaterialBind(material);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmaterialBindSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmaterial* material = JS_GET_ARG_PTR(args, 0, OPmaterial);
    OPmaterialBind(material);

    JS_RETURN_NULL;
}

void OPmaterialWrapperCreate(Handle<Object> result, OPmaterial* material) {
    SCOPE_AND_ISOLATE

    JS_SET_PTR(result, material);
    JS_SET_METHOD(result, "ClearParams", _OPmaterialClearParamsSelf);
    JS_SET_METHOD(result, "AddParam", _OPmaterialAddParamSelf);
    JS_SET_METHOD(result, "Bind", _OPmaterialBindSelf);
    JS_SET_NUMBER(result, "size", sizeof(OPmaterial));
}

JS_RETURN_VAL _OPmaterialCreate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmaterial* material = (OPmaterial*)OPalloc(sizeof(OPmaterial));
    OPeffect* effect = JS_GET_ARG_PTR(args, 0, OPeffect);
    OPmaterialInit(material, effect);

    Handle<Object> result = JS_NEW_OBJECT();
    OPmaterialWrapperCreate(result, material);

    JS_RETURN(result);
}

void OPmaterialWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> material = JS_NEW_OBJECT();
    JS_SET_METHOD(material, "ClearParams", _OPmaterialClearParams);
    JS_SET_METHOD(material, "Init", _OPmaterialInit);
    JS_SET_METHOD(material, "Create", _OPmaterialCreate);
    JS_SET_METHOD(material, "AddParam", _OPmaterialAddParam);
    JS_SET_METHOD(material, "Bind", _OPmaterialBind);
    JS_SET_NUMBER(material, "size", sizeof(OPmaterial));

    Handle<Object> ParamType = JS_NEW_OBJECT();
    JS_SET_NUMBER(ParamType, "MATRIX4", MATERIAL_PARAM_TYPE_MATRIX4);
    JS_SET_NUMBER(ParamType, "MATRIX4V", MATERIAL_PARAM_TYPE_MATRIX4V);
    JS_SET_NUMBER(ParamType, "TEXTURE", MATERIAL_PARAM_TYPE_TEXTURE);
    JS_SET_NUMBER(ParamType, "VECTOR3", MATERIAL_PARAM_TYPE_VECTOR3);
    JS_SET_NUMBER(ParamType, "VECTOR4", MATERIAL_PARAM_TYPE_VECTOR4);
    JS_SET_OBJECT(material, "ParamType", ParamType);

    JS_SET_OBJECT(exports, "material", material);

}

#endif

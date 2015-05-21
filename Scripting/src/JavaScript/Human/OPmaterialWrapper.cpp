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

JS_RETURN_VAL _OPmaterialInit(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmaterial* material = JS_GET_ARG_PTR(args, 0, OPmaterial);
    OPeffect* effect = JS_GET_ARG_PTR(args, 1, OPeffect);
    OPmaterialInit(material, effect);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmaterialCreate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmaterial* material = (OPmaterial*)OPalloc(sizeof(OPmaterial));
    OPeffect* effect = JS_GET_ARG_PTR(args, 0, OPeffect);
    OPmaterialInit(material, effect);

    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_PTR(result, material);

    JS_RETURN(result);
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

JS_RETURN_VAL _OPmaterialBind(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmaterial* material = JS_GET_ARG_PTR(args, 0, OPmaterial);
    OPmaterialBind(material);

    JS_RETURN_NULL;
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
    JS_SET_OBJECT(exports, "material", material);

}

#endif
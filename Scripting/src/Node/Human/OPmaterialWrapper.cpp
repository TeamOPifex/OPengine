// myobject.cc
#include "./Scripting/include/Node/Human/Wrappers.h"
#include "./Scripting/include/Node/Math/Wrappers.h"

#include "./Human/Human.h"


NODE_RETURN_VAL _OPmaterialClearParams(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmaterial* material = NODE_GET_ARG_PTR(args, 0, OPmaterial);
    OPmaterialClearParams(material);

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPmaterialInit(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmaterial* material = NODE_GET_ARG_PTR(args, 0, OPmaterial);
    OPeffect* effect = NODE_GET_ARG_PTR(args, 1, OPeffect);
    OPmaterialInit(material, effect);

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPmaterialCreate(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmaterial* material = (OPmaterial*)OPalloc(sizeof(OPmaterial));
    OPeffect* effect = NODE_GET_ARG_PTR(args, 0, OPeffect);
    OPmaterialInit(material, effect);

    Handle<Object> result = NODE_NEW_OBJECT();
    NODE_SET_PTR(result, material);

    NODE_RETURN(result);
}

NODE_RETURN_VAL _OPmaterialAddParam(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmaterial* material = NODE_GET_ARG_PTR(args, 0, OPmaterial);
    enum OPmaterialParamType matParamType = (enum OPmaterialParamType)(OPint)args[1]->NumberValue();
    String::Utf8Value str(args[2]->ToString());
    void* data = (void*)(OPint)args[3]->NumberValue();
    ui8 count = (ui8)args[4]->NumberValue();
    OPmaterialAddParam(material, matParamType, *str, data, count);

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPmaterialBind(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmaterial* material = NODE_GET_ARG_PTR(args, 0, OPmaterial);
    OPmaterialBind(material);

    NODE_RETURN_NULL;
}

void OPmaterialWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> material = NODE_NEW_OBJECT();
    NODE_SET_METHOD(material, "ClearParams", _OPmaterialClearParams);
    NODE_SET_METHOD(material, "Init", _OPmaterialInit);
    NODE_SET_METHOD(material, "Create", _OPmaterialCreate);
    NODE_SET_METHOD(material, "AddParam", _OPmaterialAddParam);
    NODE_SET_METHOD(material, "Bind", _OPmaterialBind);
    NODE_SET_NUMBER(material, "size", sizeof(OPmaterial));
    NODE_SET_OBJECT(exports, "material", material);

}
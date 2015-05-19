// myobject.cc
#include "./Scripting/include/Node/Human/Wrappers.h"
#include "./Scripting/include/Node/Math/Wrappers.h"

#include "./Human/Human.h"

NODE_RETURN_VAL _OPfontSystemsLoadEffects(const NODE_ARGS& args) {
    OPfontSystemLoadEffects();
}

NODE_RETURN_VAL _OPfontManagerScale(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPfontManager* fontManager = NODE_GET_PTR(args.This(), OPfontManager);
    fontManager->scale = args[0]->NumberValue();

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPfontManagerSetup(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPfontManager* manager = OPfontManagerSetup("Ubuntu.opf", NULL, 0);
    Handle<Object> result = NODE_NEW_OBJECT();
    NODE_SET_PTR(result, manager);
    NODE_SET_METHOD(result, "scale", _OPfontManagerScale);

    NODE_RETURN(result);
}

NODE_RETURN_VAL _OPfontManagerColor(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPfontManager* fontManager = NODE_GET_ARG_PTR(args, 0, OPfontManager);
    OPfontManagerSetColor(fontManager, args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue(), 1.0);

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPfontRenderBegin(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    Handle<Object> obj = args[0]->ToObject();
    OPfontManager* fontManager = NODE_GET_PTR(obj, OPfontManager);
    fontManager->scale = NODE_GET_NUMBER(obj, "scale");
    OPfontRenderBegin(fontManager);

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPfontRenderColor(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPfontColor(OPvec4Create(args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue(), 1.0));

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPfontRenderText(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    String::Utf8Value str(args[0]->ToString());
    OPfontRender(*str, OPvec2Create(args[1]->NumberValue(), args[2]->NumberValue()));

    NODE_RETURN_NULL;
}

NODE_RETURN_VAL _OPfontRenderEnd(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPfontRenderEnd();

    NODE_RETURN_NULL;
}

void OPfontWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> font = NODE_NEW_OBJECT();
    NODE_SET_METHOD(font, "LoadEffects", _OPfontSystemsLoadEffects);

    { // OP.font.Manager
        Handle<Object> manager = NODE_NEW_OBJECT();
        NODE_SET_METHOD(manager, "Setup", _OPfontManagerSetup);
        NODE_SET_METHOD(manager, "Color", _OPfontManagerColor);
        NODE_SET_NUMBER(manager, "size", sizeof(OPfontManager));
        NODE_SET_OBJECT(font, "Manager", manager);
    }

    { // OP.font.Render
        Handle<Object> render = NODE_NEW_OBJECT();
        NODE_SET_METHOD(render, "Begin", _OPfontRenderBegin);
        NODE_SET_METHOD(render, "Color", _OPfontRenderColor);
        NODE_SET_METHOD(render, "Text", _OPfontRenderText);
        NODE_SET_METHOD(render, "End", _OPfontRenderEnd);
        NODE_SET_OBJECT(font, "Render", render);
    }

    NODE_SET_OBJECT(exports, "font", font);

}
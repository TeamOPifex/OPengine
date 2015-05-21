// myobject.cc
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Human/Human.h"

JS_RETURN_VAL _OPfontSystemsLoadEffects(const JS_ARGS& args) {
    OPfontSystemLoadEffects();
}

JS_RETURN_VAL _OPfontManagerScale(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPfontManager* fontManager = JS_GET_PTR(args.This(), OPfontManager);
    fontManager->scale = args[0]->NumberValue();

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPfontManagerSetup(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPfontManager* manager = OPfontManagerSetup("Ubuntu.opf", NULL, 0);
    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_PTR(result, manager);
    JS_SET_METHOD(result, "scale", _OPfontManagerScale);

    JS_RETURN(result);
}

JS_RETURN_VAL _OPfontManagerColor(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPfontManager* fontManager = JS_GET_ARG_PTR(args, 0, OPfontManager);
    OPfontManagerSetColor(fontManager, args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue(), 1.0);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPfontRenderBegin(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    Handle<Object> obj = args[0]->ToObject();
    OPfontManager* fontManager = JS_GET_PTR(obj, OPfontManager);
    fontManager->scale = JS_GET_NUMBER(obj, "scale");
    OPfontRenderBegin(fontManager);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPfontRenderColor(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPfontColor(OPvec4Create(args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue(), 1.0));

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPfontRenderText(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    String::Utf8Value str(args[0]->ToString());
    OPfontRender(*str, OPvec2Create(args[1]->NumberValue(), args[2]->NumberValue()));

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPfontRenderEnd(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPfontRenderEnd();

    JS_RETURN_NULL;
}

void OPfontWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> font = JS_NEW_OBJECT();
    JS_SET_METHOD(font, "LoadEffects", _OPfontSystemsLoadEffects);

    { // OP.font.Manager
        Handle<Object> manager = JS_NEW_OBJECT();
        JS_SET_METHOD(manager, "Setup", _OPfontManagerSetup);
        JS_SET_METHOD(manager, "Color", _OPfontManagerColor);
        JS_SET_NUMBER(manager, "size", sizeof(OPfontManager));
        JS_SET_OBJECT(font, "Manager", manager);
    }

    { // OP.font.Render
        Handle<Object> render = JS_NEW_OBJECT();
        JS_SET_METHOD(render, "Begin", _OPfontRenderBegin);
        JS_SET_METHOD(render, "Color", _OPfontRenderColor);
        JS_SET_METHOD(render, "Text", _OPfontRenderText);
        JS_SET_METHOD(render, "End", _OPfontRenderEnd);
        JS_SET_OBJECT(font, "Render", render);
    }

    JS_SET_OBJECT(exports, "font", font);

}

#endif
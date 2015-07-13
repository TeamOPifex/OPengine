// myobject.cc
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"
#include "./Data/include/OPcman.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Human/Human.h"

JS_RETURN_VAL _OPfontSystemsLoadEffects(const JS_ARGS& args) {
    OPfontSystemLoadEffects();
    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPfontManagerScale(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPfontManager* fontManager = JS_GET_PTR(args.This(), OPfontManager);
    fontManager->scale = args[0]->NumberValue();

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPfontManagerSetup(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    String::Utf8Value font(args[0]->ToString());
    OPfontManager* manager = OPfontManagerSetup(*font, NULL, 0);

    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_PTR(result, manager);

    JS_RETURN(result);
}

JS_RETURN_VAL _OPfontManagerCreate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPfont* font = JS_GET_ARG_PTR(args, 0, OPfont);

    OPfontManager* manager = OPfontManagerCreate(font);

    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_PTR(result, manager);

    JS_RETURN(result);
}

JS_RETURN_VAL _OPfontManagerDestroy(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPfontManager* ptr = JS_GET_ARG_PTR(args, 0, OPfontManager);

    OPfontManagerDestroy(ptr);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPfontManagerColor(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPfontManager* fontManager = JS_GET_ARG_PTR(args, 0, OPfontManager);
    OPfontManagerSetColor(fontManager, args[1]->NumberValue(), args[2]->NumberValue(), args[3]->NumberValue(), 1.0);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPfontRenderBegin(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPfontManager* fontManager = JS_GET_ARG_PTR(args, 0, OPfontManager);
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
    OPvec2 pos = OPvec2Create(args[1]->NumberValue(), args[2]->NumberValue());
    OPfontRender(*str, pos);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPfontRenderEnd(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPfontRenderEnd();

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPfontLoad(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    String::Utf8Value str(args[0]->ToString());
    OPfont* font = (OPfont*)OPcmanLoadGet(*str);

    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_PTR(result, font);

    Handle<Object> texture = JS_NEW_OBJECT();
    JS_SET_PTR(texture, font->texture);
    JS_SET_OBJECT(result, "texture", texture);

    JS_RETURN(result);
}



void OPfontWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> font = JS_NEW_OBJECT();
    JS_SET_METHOD(font, "LoadEffects", _OPfontSystemsLoadEffects);
    JS_SET_METHOD(font, "Load", _OPfontLoad);

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


    Handle<Object> fontManager = JS_NEW_OBJECT();
    JS_SET_METHOD(fontManager, "Create", _OPfontManagerCreate);
    JS_SET_METHOD(fontManager, "Destroy", _OPfontManagerDestroy);
    JS_SET_OBJECT(exports, "fontManager", fontManager);

}

#endif

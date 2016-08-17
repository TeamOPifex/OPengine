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

JS_RETURN_VAL _OPfontLoad(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    String::Utf8Value str(args[0]->ToString());
    OPfont* font = (OPfont*)OPCMAN.LoadGet(*str);

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

    JS_SET_OBJECT(exports, "font", font);

}

#endif

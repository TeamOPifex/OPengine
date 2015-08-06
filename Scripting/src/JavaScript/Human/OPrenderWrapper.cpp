// myobject.cc
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Human/Human.h"

// OP.render.Init
JS_RETURN_VAL _OPrenderInit(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPrenderInit();

    JS_RETURN_NULL;
}

// OP.render.Clear
JS_RETURN_VAL _OPrenderClear(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPrenderClear(
    args[0]->NumberValue(),
    args[1]->NumberValue(),
    args[2]->NumberValue());

    JS_RETURN_NULL;
}

// OP.render.Present
JS_RETURN_VAL _OPrenderPresent(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPrenderPresent();

    JS_RETURN_NULL;
}

// OP.render.Size
JS_RETURN_VAL _OPrenderSize(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    Handle<Object> obj = JS_NEW_OBJECT();
    JS_SET_NUMBER(obj, "Width", OPRENDER_WIDTH);
    JS_SET_NUMBER(obj, "Height", OPRENDER_HEIGHT);
    JS_SET_NUMBER(obj, "ScreenWidth", OPRENDER_SCREEN_WIDTH);
    JS_SET_NUMBER(obj, "ScreenHeight", OPRENDER_SCREEN_HEIGHT);
    JS_SET_NUMBER(obj, "ScaledWidth", OPRENDER_SCALED_WIDTH);
    JS_SET_NUMBER(obj, "ScaledHeight", OPRENDER_SCALED_HEIGHT);

    JS_RETURN(obj);
}

JS_RETURN_VAL _OPrenderCull(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPrenderCull(args[0]->IntegerValue());

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPrenderCullMode(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPrenderCullMode(args[0]->IntegerValue());

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPrenderDepth(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPrenderDepth(args[0]->IntegerValue());

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPrenderDepthWrite(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPrenderDepthWrite(args[0]->IntegerValue());

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPrenderFullScreen(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPRENDER_FULLSCREEN = args[0]->IntegerValue();

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPrenderSetScreenSize(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPRENDER_WIDTH = args[0]->IntegerValue();
    OPRENDER_HEIGHT = args[0]->IntegerValue();

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPrenderBlend(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPrenderBlend(args[0]->IntegerValue());

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPrenderBlendAlpha(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPrenderBlendAlpha();

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPrenderBlendAdditive(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPrenderBlendAdditive();

    JS_RETURN_NULL;
}

void OPrenderWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> render = JS_NEW_OBJECT();
    JS_SET_METHOD(render, "Init", _OPrenderInit);
    JS_SET_METHOD(render, "Clear", _OPrenderClear);
    JS_SET_METHOD(render, "Present", _OPrenderPresent);
    JS_SET_METHOD(render, "SetScreenSize", _OPrenderSetScreenSize);
    JS_SET_METHOD(render, "Size", _OPrenderSize);
    JS_SET_METHOD(render, "Cull", _OPrenderCull);
    JS_SET_METHOD(render, "CullMode", _OPrenderCullMode);
    JS_SET_METHOD(render, "Depth", _OPrenderDepth);
    JS_SET_METHOD(render, "DepthWrite", _OPrenderDepthWrite);
    JS_SET_METHOD(render, "Blend", _OPrenderBlend);
    JS_SET_METHOD(render, "FullScreen", _OPrenderFullScreen);
    JS_SET_METHOD(render, "BlendAlpha", _OPrenderBlendAlpha);
    JS_SET_METHOD(render, "BlendAdditive", _OPrenderBlendAdditive);
    JS_SET_OBJECT(exports, "render", render);

}

#endif

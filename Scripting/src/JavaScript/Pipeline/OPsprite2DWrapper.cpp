// myobject.cc
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Pipeline/Pipeline.h"

JS_RETURN_VAL _OPsprite2DInit(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPsprite2DInit(NULL);

    JS_RETURN_NULL
}

JS_RETURN_VAL _OPsprite2DCreate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPsprite** sprites;

    OPsprite2D* ptr = OPsprite2DCreate(sprites, NULL);

    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_PTR(result, ptr);

    JS_RETURN(result);
}

JS_RETURN_VAL _OPsprite2DDestroy(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPsprite2D* ptr = JS_GET_ARG_PTR(args, 0, OPsprite2D);
    OPsprite2DDestroy(ptr);

    JS_RETURN_NULL
}

JS_RETURN_VAL _OPsprite2DPrepRender(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPsprite2D* ptr = JS_GET_ARG_PTR(args, 0, OPsprite2D);
    OPsprite2DPrepRender(ptr);

    JS_RETURN_NULL
}

JS_RETURN_VAL _OPsprite2DRender(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPsprite2D* ptr = JS_GET_ARG_PTR(args, 0, OPsprite2D);
    OPsprite2DRender(ptr);

    JS_RETURN_NULL
}

JS_RETURN_VAL _OPsprite2DUpdate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPsprite2D* ptr = JS_GET_ARG_PTR(args, 0, OPsprite2D);
    OPtimer* timer = JS_GET_ARG_PTR(args, 1, OPtimer);
    OPsprite2DUpdate(ptr, timer);

    JS_RETURN_NULL
}

JS_RETURN_VAL _OPsprite2DSetSprite(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPsprite2D* ptr = JS_GET_ARG_PTR(args, 0, OPsprite2D);
    OPsprite2DSetSprite(ptr, args[1]->IntegerValue());

    JS_RETURN_NULL
}

void OPsprite2DWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> sprite2D = JS_NEW_OBJECT();
    JS_SET_METHOD(sprite2D, "Init", _OPsprite2DInit);
    JS_SET_METHOD(sprite2D, "Create", _OPsprite2DCreate);
    JS_SET_METHOD(sprite2D, "Destroy", _OPsprite2DDestroy);
    JS_SET_METHOD(sprite2D, "SetSprite", _OPsprite2DSetSprite);
    JS_SET_METHOD(sprite2D, "PrepRender", _OPsprite2DPrepRender);
    JS_SET_METHOD(sprite2D, "Render", _OPsprite2DRender);
    JS_SET_NUMBER(sprite2D, "size", sizeof(OPsprite2D));
    JS_SET_OBJECT(exports, "sprite2D", sprite2D);
}

#endif

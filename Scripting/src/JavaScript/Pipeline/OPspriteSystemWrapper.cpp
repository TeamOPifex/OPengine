// myobject.cc
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Pipeline/Pipeline.h"

// JS_RETURN_VAL _OPspriteSystemInit(const JS_ARGS& args) {
//     SCOPE_AND_ISOLATE
//
//     OPspriteSystemInit(NULL);
//
//     JS_RETURN_NULL
// }

JS_RETURN_VAL _OPspriteSystemCreate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPint count;
    OPsprite** sprites;

    Local<Array> arr = Local<Array>::Cast(args[0]);
    count =  arr->Length();
    sprites = (OPsprite**)OPalloc(sizeof(OPsprite*) * count);
    for(OPuint i = 0 ; i < count; i++) {
      Local<Object> item = Local<Object>::Cast(arr->Get(i));
      sprites[i] = JS_GET_PTR(item, OPsprite);
    }

    count = args[1]->IntegerValue();
    OPspriteSystemAlign align = (OPspriteSystemAlign)args[2]->IntegerValue();

    OPspriteSystem* ptr = OPspriteSystemCreate(sprites, count, NULL, align);

    //OPfree(sprites);

    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_PTR(result, ptr);

    JS_RETURN(result);
}

JS_RETURN_VAL _OPspriteSystemAdd(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPspriteSystem* ptr = JS_GET_ARG_PTR(args, 0, OPspriteSystem);
    OPspriteSystemSprite* r = OPspriteSystemAdd(ptr);

    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_PTR(result, r);

    Handle<Object> Position = JS_NEW_OBJECT();
    OPvec2WrapperCreate(Position, &r->Position);
    JS_SET_OBJECT(result, "Position", Position);

    Handle<Object> Scale = JS_NEW_OBJECT();
    OPvec2WrapperCreate(Scale, &r->Scale);
    JS_SET_OBJECT(result, "Scale", Scale);

    JS_RETURN(result);
}

JS_RETURN_VAL _OPspriteSystemRemove(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPspriteSystem* ptr = JS_GET_ARG_PTR(args, 0, OPspriteSystem);
    OPspriteSystemSprite* r = JS_GET_ARG_PTR(args, 0, OPspriteSystemSprite);
    OPspriteSystemRemove(ptr, r);

    JS_RETURN_NULL
}

// JS_RETURN_VAL _OPspriteSystemDestroy(const JS_ARGS& args) {
//     SCOPE_AND_ISOLATE
//
//     OPspriteSystem* ptr = JS_GET_ARG_PTR(args, 0, OPspriteSystem);
//     OPspriteSystemDestroy(ptr);
//
//     JS_RETURN_NULL
// }

JS_RETURN_VAL _OPspriteSystemUpdate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPspriteSystem* ptr = JS_GET_ARG_PTR(args, 0, OPspriteSystem);
    OPtimer* timer = JS_GET_ARG_PTR(args, 1, OPtimer);
    OPspriteSystemUpdate(ptr, timer);

    JS_RETURN_NULL
}

JS_RETURN_VAL _OPspriteSystemRender(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPspriteSystem* ptr = JS_GET_ARG_PTR(args, 0, OPspriteSystem);
    OPcam* cam = JS_GET_ARG_PTR(args, 1, OPcam);
    OPspriteSystemRender(ptr, cam);

    JS_RETURN_NULL
}

JS_RETURN_VAL _OPspriteSystemSetSprite(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPspriteSystemSprite* sprite = JS_GET_ARG_PTR(args, 0, OPspriteSystemSprite);
    sprite->CurrentSprite = args[1]->IntegerValue();

    JS_RETURN_NULL
}

JS_RETURN_VAL _OPspriteSystemCurrent(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPspriteSystem* ptr = JS_GET_ARG_PTR(args, 0, OPspriteSystem);
    OPspriteSystemSprite* sprite = JS_GET_ARG_PTR(args, 1, OPspriteSystemSprite);
    OPsprite* r = OPspriteSystemCurrent(ptr, sprite);

    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_PTR(result, r);

    JS_RETURN(result);
}

void OPspriteSystemWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> result = JS_NEW_OBJECT();
    //JS_SET_METHOD(result, "Init", _OPspriteSystemInit);
    JS_SET_METHOD(result, "Create", _OPspriteSystemCreate);
    JS_SET_METHOD(result, "Add", _OPspriteSystemAdd);
    JS_SET_METHOD(result, "Remove", _OPspriteSystemRemove);
    JS_SET_METHOD(result, "Update", _OPspriteSystemUpdate);
    JS_SET_METHOD(result, "Render", _OPspriteSystemRender);
    JS_SET_METHOD(result, "SetSprite", _OPspriteSystemSetSprite);
    //JS_SET_METHOD(result, "Destroy", _OPspriteSystemDestroy);
    JS_SET_METHOD(result, "Current", _OPspriteSystemCurrent);
    JS_SET_NUMBER(result, "size", sizeof(OPspriteSystem));
    JS_SET_OBJECT(exports, "spriteSystem", result);


    Handle<Object> align = JS_NEW_OBJECT();
    JS_SET_NUMBER(align, "CENTER", OPSPRITESYSTEMALIGN_CENTER);
    JS_SET_NUMBER(align, "BOTTOM_CENTER", OPSPRITESYSTEMALIGN_BOTTOM_CENTER);
    JS_SET_NUMBER(align, "VERTICAL_CENTER", OPSPRITESYSTEMALIGN_VERTICAL_CENTER);
    JS_SET_NUMBER(align, "BOTTOM_LEFT", OPSPRITESYSTEMALIGN_BOTTOM_LEFT);
    JS_SET_NUMBER(align, "TOP_RIGHT", OPSPRITESYSTEMALIGN_TOP_RIGHT);
    JS_SET_OBJECT(exports, "SPRITESYSTEMALIGN", align);
}

#endif

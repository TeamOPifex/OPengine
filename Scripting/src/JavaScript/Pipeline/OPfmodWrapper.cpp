// myobject.cc
#include "./Scripting/include/JavaScript/Pipeline/Wrappers.h"
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#ifdef OPIFEX_OPTION_FMOD

#include "./Pipeline/Pipeline.h"

JS_RETURN_VAL _OPfmodInit(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPfmodInit();

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPfmodPlay(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPfmodSound* ptr = JS_GET_ARG_PTR(args, 0, OPfmodSound);
    OPfmodChannel* r = OPfmodPlay(ptr);


    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_PTR(result, r);

    JS_RETURN(result);
}

JS_RETURN_VAL _OPfmodPlaySelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPfmodSound* ptr = JS_GET_PTR(args.This(), OPfmodSound);
    OPfmodChannel* r = OPfmodPlay(ptr);

    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_PTR(result, r);

    JS_RETURN(result);
}

JS_RETURN_VAL _OPfmodPlayChannelGroup(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPfmodSound* ptr = JS_GET_ARG_PTR(args, 0, OPfmodSound);
    OPfmodChannelGroup* group = JS_GET_ARG_PTR(args, 1, OPfmodChannelGroup);
    OPfmodChannel* r = OPfmodPlay(ptr, group);

    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_PTR(result, r);

    JS_RETURN(result);
}

JS_RETURN_VAL _OPfmodPlayChannelGroupSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPfmodSound* ptr = JS_GET_PTR(args.This(), OPfmodSound);
    OPfmodChannelGroup* group = JS_GET_ARG_PTR(args, 0, OPfmodChannelGroup);
    OPfmodChannel* r = OPfmodPlay(ptr, group);

    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_PTR(result, r);

    JS_RETURN(result);
}

JS_RETURN_VAL _OPfmodCreate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    String::Utf8Value str(args[0]->ToString());
    OPfmodSound* ptr = OPfmodLoad(*str);

    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_PTR(result, ptr);

    JS_SET_METHOD(result, "Play", _OPfmodPlaySelf);

    JS_RETURN(result);
}

// JS_RETURN_VAL _OPfmodCreateSound(const JS_ARGS& args) {
//     SCOPE_AND_ISOLATE
//
//     OPfmodSound* ptr = (OPfmodSound*)OPalloc(sizeof(OPfmodSound));
//
//     String::Utf8Value str(args[0]->ToString());
//     OPfmodLoad(ptr, *str);
//
//     Handle<Object> result = JS_NEW_OBJECT();
//     JS_SET_PTR(result, ptr);
//
//     JS_SET_METHOD(result, "Play", _OPfmodPlaySelf);
//
//     JS_RETURN(result);
// }

JS_RETURN_VAL _OPfmodCreateChannelGroup(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    String::Utf8Value str(args[0]->ToString());
    OPfmodChannelGroup* ptr = OPfmodCreateChannelGroup(*str);

    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_PTR(result, ptr);

    JS_RETURN(result);
}

JS_RETURN_VAL _OPfmodLoad(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    String::Utf8Value str(args[0]->ToString());
    OPfmodSound* ptr = OPfmodLoad(*str);

    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_PTR(result, ptr);

    JS_RETURN(result);
}

JS_RETURN_VAL _OPfmodUpdate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPfmodUpdate();

    JS_RETURN_NULL
}

JS_RETURN_VAL _OPfmodIsPlaying(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPfmodChannel* ptr = JS_GET_ARG_PTR(args, 0, OPfmodChannel);

    bool result = OPfmodIsPlaying(ptr);

    JS_RETURN(JS_NEW_NUMBER(result));
}

JS_RETURN_VAL _OPfmodSetVolume(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPfmodChannel* ptr = JS_GET_ARG_PTR(args, 0, OPfmodChannel);

    OPfmodSetVolume(ptr, (f32)args[1]->NumberValue());

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPfmodSetChannelGroupVolume(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPfmodChannelGroup* ptr = JS_GET_ARG_PTR(args, 0, OPfmodChannelGroup);
    OPfmodSetVolume(ptr, (f32)args[1]->NumberValue());

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPfmodSetPause(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPfmodChannel* ptr = JS_GET_ARG_PTR(args, 0, OPfmodChannel);

    OPfmodSetPause(ptr, args[1]->IntegerValue() != 0);

    JS_RETURN_NULL;
}
#endif

void OPfmodWrapper(Handle<Object> exports) {
#ifdef OPIFEX_OPTION_FMOD
    SCOPE_AND_ISOLATE;

    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_METHOD(result, "Init", _OPfmodInit);
    JS_SET_METHOD(result, "Create", _OPfmodCreate);
    JS_SET_METHOD(result, "CreateChannelGroup", _OPfmodCreateChannelGroup);
    JS_SET_METHOD(result, "Load", _OPfmodLoad);
    JS_SET_METHOD(result, "Play", _OPfmodPlay);
    JS_SET_METHOD(result, "PlayChannelGroup", _OPfmodPlayChannelGroup);
    JS_SET_METHOD(result, "SetPause", _OPfmodSetPause);
    JS_SET_METHOD(result, "SetVolume", _OPfmodSetVolume);
    JS_SET_METHOD(result, "SetChannelGroupVolume", _OPfmodSetChannelGroupVolume);
    JS_SET_METHOD(result, "Update", _OPfmodUpdate);
    JS_SET_METHOD(result, "IsPlaying", _OPfmodIsPlaying);
    JS_SET_NUMBER(result, "size", sizeof(OPfmod));
    JS_SET_OBJECT(exports, "fmod", result);
#endif
}

#endif

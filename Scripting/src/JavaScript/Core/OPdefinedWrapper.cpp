// myobject.cc
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)


void _SetDefines(Handle<Object> obj) {
    SCOPE_AND_ISOLATE

#ifdef OPIFEX_OPTION_MYO
        JS_SET_NUMBER(obj, "OPIFEX_OPTION_MYO", 1);
    #endif
#ifdef OPIFEX_OPTION_OCULUS
        JS_SET_NUMBER(obj, "OPIFEX_OPTION_OCULUS", 1);
    #endif
#ifdef OPIFEX_OPTION_PHYSICS
        JS_SET_NUMBER(obj, "OPIFEX_OPTION_PHYSICS", 1);
    #endif
#ifdef OPIFEX_OPTION_RELEASE
        JS_SET_NUMBER(obj, "OPIFEX_OPTION_RELEASE", 1);
    #endif
#ifdef OPIFEX_OPTION_SHARED
        JS_SET_NUMBER(obj, "OPIFEX_OPTION_SHARED", 1);
    #endif
#ifdef OPIFEX_OPTION_SPINE
        JS_SET_NUMBER(obj, "OPIFEX_OPTION_SPINE", 1);
    #endif
#ifdef OPIFEX_OPTION_FMOD
        JS_SET_NUMBER(obj, "OPIFEX_OPTION_FMOD", 1);
    #endif
#ifdef OPIFEX_OPTION_AUDIO
        JS_SET_NUMBER(obj, "OPIFEX_OPTION_AUDIO", 1);
    #endif
#ifdef OPIFEX_OPTION_V8
        JS_SET_NUMBER(obj, "OPIFEX_OPTION_V8", 1);
    #endif
#ifdef OPIFEX_GL_2_0
        JS_SET_NUMBER(obj, "OPIFEX_GL_2_0", 1);
    #endif
#ifdef OPIFEX_GL_3_3
        JS_SET_NUMBER(obj, "OPIFEX_GL_3_3", 1);
    #endif

#ifdef OPIFEX_OSX32
        JS_SET_NUMBER(obj, "OPIFEX_OSX32", 1);
    #endif
#ifdef OPIFEX_OSX64
            JS_SET_NUMBER(obj, "OPIFEX_OSX64", 1);
#endif
#ifdef OPIFEX_OSX
    JS_SET_NUMBER(obj, "OPIFEX_OSX", 1);
#endif
#ifdef OPIFEX_LINUX32
        JS_SET_NUMBER(obj, "OPIFEX_LINUX32", 1);
    #endif
#ifdef OPIFEX_LINUX64
        JS_SET_NUMBER(obj, "OPIFEX_LINUX64", 1);
    #endif
#ifdef OPIFEX_LINUX
        JS_SET_NUMBER(obj, "OPIFEX_LINUX", 1);
    #endif
#ifdef OPIFEX_WIN32
        JS_SET_NUMBER(obj, "OPIFEX_WIN32", 1);
    #endif
#ifdef OPIFEX_WIN64
        JS_SET_NUMBER(obj, "OPIFEX_WIN64", 1);
    #endif
#ifdef OPIFEX_WINDOWS
        JS_SET_NUMBER(obj, "OPIFEX_WINDOWS", 1);
    #endif
#ifdef OPIFEX_ANDROID
        JS_SET_NUMBER(obj, "OPIFEX_ANDROID", 1);
    #endif
#ifdef OPIFEX_UNIX
    JS_SET_NUMBER(obj, "OPIFEX_UNIX", 1);
#endif
#ifdef OPIFEX_OS32
        JS_SET_NUMBER(obj, "OPIFEX_OS32", 1);
    #endif
#ifdef OPIFEX_OS64
    JS_SET_NUMBER(obj, "OPIFEX_OS64", 1);
#endif
}

void OPdefinedWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> defined = JS_NEW_OBJECT();
    _SetDefines(defined);
    JS_SET_OBJECT(exports, "defined", defined);
}
#endif
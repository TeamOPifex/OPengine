// myobject.cc
#include "./Scripting/include/Node/Human/Wrappers.h"
#include "./Scripting/include/Node/Math/Wrappers.h"

#include "./Core/Core.h"


void _SetDefines(Handle<Object> obj) {
    SCOPE_AND_ISOLATE

#ifdef OPIFEX_OPTION_MYO
        NODE_SET_NUMBER(obj, "OPIFEX_OPTION_MYO", 1);
    #endif
#ifdef OPIFEX_OPTION_OCULUS
        NODE_SET_NUMBER(obj, "OPIFEX_OPTION_OCULUS", 1);
    #endif
#ifdef OPIFEX_OPTION_PHYSICS
        NODE_SET_NUMBER(obj, "OPIFEX_OPTION_PHYSICS", 1);
    #endif
#ifdef OPIFEX_OPTION_RELEASE
        NODE_SET_NUMBER(obj, "OPIFEX_OPTION_RELEASE", 1);
    #endif
#ifdef OPIFEX_OPTION_SHARED
        NODE_SET_NUMBER(obj, "OPIFEX_OPTION_SHARED", 1);
    #endif
#ifdef OPIFEX_OPTION_SPINE
        NODE_SET_NUMBER(obj, "OPIFEX_OPTION_SPINE", 1);
    #endif
#ifdef OPIFEX_OPTION_FMOD
        NODE_SET_NUMBER(obj, "OPIFEX_OPTION_FMOD", 1);
    #endif
#ifdef OPIFEX_OPTION_AUDIO
        NODE_SET_NUMBER(obj, "OPIFEX_OPTION_AUDIO", 1);
    #endif
#ifdef OPIFEX_OPTION_V8
        NODE_SET_NUMBER(obj, "OPIFEX_OPTION_V8", 1);
    #endif
#ifdef OPIFEX_GL_2_0
        NODE_SET_NUMBER(obj, "OPIFEX_GL_2_0", 1);
    #endif
#ifdef OPIFEX_GL_3_3
        NODE_SET_NUMBER(obj, "OPIFEX_GL_3_3", 1);
    #endif

#ifdef OPIFEX_OSX32
        NODE_SET_NUMBER(obj, "OPIFEX_OSX32", 1);
    #endif
#ifdef OPIFEX_OSX64
            NODE_SET_NUMBER(obj, "OPIFEX_OSX64", 1);
#endif
#ifdef OPIFEX_OSX
    NODE_SET_NUMBER(obj, "OPIFEX_OSX", 1);
#endif
#ifdef OPIFEX_LINUX32
        NODE_SET_NUMBER(obj, "OPIFEX_LINUX32", 1);
    #endif
#ifdef OPIFEX_LINUX64
        NODE_SET_NUMBER(obj, "OPIFEX_LINUX64", 1);
    #endif
#ifdef OPIFEX_LINUX
        NODE_SET_NUMBER(obj, "OPIFEX_LINUX", 1);
    #endif
#ifdef OPIFEX_WIN32
        NODE_SET_NUMBER(obj, "OPIFEX_WIN32", 1);
    #endif
#ifdef OPIFEX_WIN64
        NODE_SET_NUMBER(obj, "OPIFEX_WIN64", 1);
    #endif
#ifdef OPIFEX_WINDOWS
        NODE_SET_NUMBER(obj, "OPIFEX_WINDOWS", 1);
    #endif
#ifdef OPIFEX_ANDROID
        NODE_SET_NUMBER(obj, "OPIFEX_ANDROID", 1);
    #endif
#ifdef OPIFEX_UNIX
    NODE_SET_NUMBER(obj, "OPIFEX_UNIX", 1);
#endif
#ifdef OPIFEX_OS32
        NODE_SET_NUMBER(obj, "OPIFEX_OS32", 1);
    #endif
#ifdef OPIFEX_OS64
    NODE_SET_NUMBER(obj, "OPIFEX_OS64", 1);
#endif
}

void OPdefinedWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> defined = NODE_NEW_OBJECT();
    _SetDefines(defined);
    NODE_SET_OBJECT(exports, "defined", defined);
}
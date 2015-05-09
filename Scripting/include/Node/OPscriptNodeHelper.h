#ifndef OPIFEXENGINE_OPSCRIPTNODEHELPER
#define OPIFEXENGINE_OPSCRIPTNODEHELPER

#include "./Core/include/OPtypes.h"
#ifdef OPIFEX_OPTION_NODEJS

#include <node.h>
#include <v8.h>

using namespace v8;

#define SCOPE_AND_ISOLATE   Isolate* isolate = Isolate::GetCurrent(); \
                            HandleScope scope(isolate);

inline void NODE_SET_OBJECT(Handle<Object>& recv, const char* name, Handle<Object>& targ) {
    SCOPE_AND_ISOLATE
    recv->Set(String::NewFromUtf8(isolate, name), targ);
}

inline Handle<Object> NODE_NEW_OBJECT() {
    SCOPE_AND_ISOLATE
    return Object::New(isolate);
}

#endif

#endif //OPIFEXENGINE_OPSCRIPTNODEWRAPPERMATH_H

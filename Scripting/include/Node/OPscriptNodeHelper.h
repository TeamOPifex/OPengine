#ifndef OPIFEXENGINE_OPSCRIPTNODEHELPER
#define OPIFEXENGINE_OPSCRIPTNODEHELPER

#include "./Core/include/OPtypes.h"
#ifdef OPIFEX_OPTION_NODEJS

#include <node.h>
#include <v8.h>

using namespace v8;




// SPECIFIC TO NODEJS 0.10.x
#ifdef OPIFEX_NODEJS_0_10

#define SCOPE_AND_ISOLATE   Isolate* isolate = Isolate::GetCurrent(); \
                            HandleScope scope;

typedef Arguments NODE_ARGS;
typedef Handle<Value> NODE_RETURN_VAL;

#define NODE_NEW_OBJECT() Object::New()
#define NODE_NEW_STRING(name) String::New(name)
#define NODE_NEW_NUMBER(val) Number::New(val)
#define NODE_NEW_BOOL(val) Boolean::New(val)
#define NODE_NEW_NULL() Null()
#define NODE_RETURN(val) return val;
#define NODE_RETURN_NULL NODE_RETURN(NODE_NEW_NULL())

#define NODE_SET_METHOD(obj, name, func) obj->Set(String::New(name), FunctionTemplate::New(func)->GetFunction());
#define NODE_SET_OBJECT(target, name, object) target->Set(NODE_NEW_STRING(name), object);


#endif




// SPECIFIC TO NODEJS 0.12.x
#ifdef OPIFEX_NODEJS_0_12

#define SCOPE_AND_ISOLATE   Isolate* isolate = Isolate::GetCurrent(); \
                            HandleScope scope(isolate);


typedef FunctionCallbackInfo<Value> NODE_ARGS;
typedef void NODE_RETURN_VAL;

#define NODE_NEW_OBJECT() Object::New(isolate)
#define NODE_NEW_STRING(name) String::NewFromUtf8(isolate, name)
#define NODE_NEW_NUMBER(val) Number::New(isolate, val)
#define NODE_NEW_BOOL(val) Boolean::New(isolate, val)
#define NODE_NEW_NULL() Null(isolate)
#define NODE_RETURN(val) args.GetReturnValue().Set(val);
#define NODE_RETURN_NULL ;

#define NODE_SET_OBJECT(target, name, object) target->Set(NODE_NEW_STRING(name), object);


#endif




#define NODE_SET_NUMBER(obj, name, val) obj->Set(NODE_NEW_STRING(name), NODE_NEW_NUMBER(val));
#define NODE_SET_PTR(obj, ptr) NODE_SET_NUMBER(obj, "ptr", (OPint)ptr)
#define NODE_GET_PTR(obj, type) (type*)(OPint)obj->Get(NODE_NEW_STRING("ptr"))->IntegerValue();


#endif

#endif //OPIFEXENGINE_OPSCRIPTNODEWRAPPERMATH_H

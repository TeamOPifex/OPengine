#ifndef OPIFEX_SCRIPTING_WRAPPERS_V8
#define OPIFEX_SCRIPTING_WRAPPERS_V8

#ifdef OPIFEX_V8

#include "v8.h"
using namespace v8;

#include "./Core/include/Types.h"

void LogProperties(Handle<Object> obj);
void* GetPointer(const FunctionCallbackInfo<Value>& args, i32* result, i32 expected);

const char* ToCString(const v8::String::Utf8Value& value);

#endif

#endif
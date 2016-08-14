// myobject.cc
#include "./Scripting/include/JavaScript/Data/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Data/Data.h"
#include "./Core/include/OPlog.h"

JS_RETURN_VAL _OPcmanInit(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

   if (args.Length() > 0) {
       v8::String::Utf8Value utf8(args[0]);
	   OPCMAN.Init(*utf8);
       OPlog("Assets %s", *utf8);
   } else {
	   OPCMAN.Init(NULL);
   }


    JS_RETURN_NULL
}

JS_RETURN_VAL _OPcmanLoad(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    if(args.Length() > 0) {
        v8::String::Utf8Value utf8(args[0]);
        const char* file = *utf8;
        OPint result = OPCMAN.Load(file);

        JS_RETURN(JS_NEW_NUMBER(result));
    }
    JS_RETURN_NULL
}

JS_RETURN_VAL _OPcmanGet(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    if(args.Length() > 0) {
        v8::String::Utf8Value utf8(args[0]);
        const char* file = *utf8;

        void* asset = OPCMAN.Get(file);

        Handle<Object> result = JS_NEW_OBJECT();
        JS_SET_PTR(result, asset);

        JS_RETURN(result);
    }
    JS_RETURN_NULL
}

JS_RETURN_VAL _OPcmanLoadGet(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    if(args.Length() > 0) {
        v8::String::Utf8Value utf8(args[0]);
        const char* file = *utf8;
        void* asset = OPCMAN.LoadGet(file);

        Handle<Object> result = JS_NEW_OBJECT();
        JS_SET_PTR(result, asset);

        JS_RETURN(result);
    }
    JS_RETURN_NULL
}

JS_RETURN_VAL _OPcmanAddLoader(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPassetLoader* ptr = JS_GET_ARG_PTR(args, 0, OPassetLoader);
	OPCMAN.AddLoader(ptr);

    JS_RETURN_NULL
}

JS_RETURN_VAL _OPcmanUpdate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPtimer* ptr = JS_GET_ARG_PTR(args, 0, OPtimer);
	OPCMAN.Update(ptr);

    JS_RETURN_NULL
}

void OPcmanWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> cman = JS_NEW_OBJECT();
    JS_SET_METHOD(cman, "Init", _OPcmanInit);
    JS_SET_METHOD(cman, "Load", _OPcmanLoad);
    JS_SET_METHOD(cman, "Update", _OPcmanUpdate);
    JS_SET_METHOD(cman, "Get", _OPcmanGet);
    JS_SET_METHOD(cman, "LoadGet", _OPcmanLoadGet);
    JS_SET_METHOD(cman, "AddLoader", _OPcmanAddLoader);
    JS_SET_OBJECT(exports, "cman", cman);
}

#endif

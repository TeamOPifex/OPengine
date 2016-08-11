// myobject.cc
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Human/Human.h"

void OPvertexLayoutWrapperCreate(Handle<Object> result, OPvertexLayout* ptr);



JS_HELPER_SELF_WRAPPER(_OPvertexLayoutInit) {
	SCOPE_AND_ISOLATE;

	OPvertexLayout* ptr = JS_NEXT_ARG_AS(OPvertexLayout);
	Handle<Value> val = JS_NEXT_ARG_VAL();
	Handle<Array> arr = JS_VALUE_TO_ARRAY(val);

	OPshaderAttribute* attributes = (OPshaderAttribute*)OPalloc(sizeof(OPshaderAttribute) * arr->Length());


	for(ui32 i = 0; i < arr->Length(); i++) {
		Handle<Value> tmp = arr->Get(i);
		Handle<Object> obj = JS_VALUE_TO_OBJECT(tmp);

		String::Utf8Value name(JS_OBJECT_GET(obj, "name")->ToString());
		OPint t = JS_OBJECT_GET_INT(obj, "type");
		OPint elements = JS_OBJECT_GET_INT(obj, "elements");
		OPuint offset = JS_OBJECT_GET_INT(obj, "offset");

		attributes[i].Name = OPstringCopy(*name);
		attributes[i].Elements = elements;
		attributes[i].Type = OPshaderElementType::FLOAT;
		//attributes[i].Offset = offset;

		OPlog("attr: %s", *name);
	}

	ptr->Init(attributes, arr->Length());

    JS_RETURN_NULL;
}



void OPvertexLayoutWrapperCreate(Handle<Object> result, OPvertexLayout* ptr) {
    SCOPE_AND_ISOLATE;

    JS_SET_PTR(result, ptr);
    JS_SET_METHOD(result, "Init", _OPvertexLayoutInitSelf);
}

JS_RETURN_VAL _OPvertexLayoutCreate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPvertexLayout* ptr = (OPvertexLayout*)OPallocZero(sizeof(OPvertexLayout));
    Handle<Object> result = JS_NEW_OBJECT();
    OPvertexLayoutWrapperCreate(result, ptr);

    JS_RETURN(result);
}

void OPvertexLayoutWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Local<FunctionTemplate> tpl = JS_NEW_FUNCTION_TEMPLATE(_OPvertexLayoutCreate);
    Handle<Object> result = tpl->GetFunction();
    JS_SET_METHOD(result, "Init", _OPvertexLayoutInit);
    JS_SET_NUMBER(result, "size", sizeof(OPvertexLayout));
    JS_SET_OBJECT(exports, "vertexLayout", result);

}

#endif

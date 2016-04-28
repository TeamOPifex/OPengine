// #include "./Scripting/include/JavaScript/Data/Wrappers.h"
//
// void OPjsonWrapperCreate(Handle<Object> result, OPcam* ptr) {
//     SCOPE_AND_ISOLATE;
//
//     JS_SET_PTR(result, ptr);
//
//     // JS_SET_METHOD(result, "Elements", _OPcamUpdateSelf);
//     // JS_SET_METHOD(result, "ArraySize", _OPcamGetViewSelf);
//     // JS_SET_METHOD(result, "Get", _OPcamGetProjSelf);
//     // JS_SET_METHOD(result, "ArrayGet", _OPcamSetPosSelf);
//     // JS_SET_METHOD(result, "String", _OPcamSetTargetSelf);
//     // JS_SET_METHOD(result, "I64", _OPcamSetTargetSelf);
//     // JS_SET_METHOD(result, "F32", _OPcamSetTargetSelf);
//     // JS_SET_METHOD(result, "Bool", _OPcamSetTargetSelf);
//     // JS_SET_METHOD(result, "Type", _OPcamSetTargetSelf);
//     // JS_SET_METHOD(result, "Log", _OPcamSetTargetSelf);
// }
//
// JS_RETURN_VAL _OPjsonWrap(const JS_ARGS& args) {
//     SCOPE_AND_ISOLATE
//
//     OPjson* ptr = JS_GET_ARG_PTR(args, 0, OPjson);
// 	Handle<Object> result = JS_NEW_OBJECT();
// 	OPjsonWrapperCreate(result, ptr);
//
// 	JS_RETURN(result);
// }
//
// JS_RETURN_VAL _OPjsonCreate(const JS_ARGS& args) {
//     SCOPE_AND_ISOLATE;
//
//     OPjson* ptr = (OPjson*)OPallocZero(sizeof(OPjson));
//     Handle<Object> result = JS_NEW_OBJECT();
//     OPjsonWrapperCreate(result, ptr);
//
//     JS_RETURN(result);
// }
//
// void OPjsonWrapper(Handle<Object> exports) {
//     SCOPE_AND_ISOLATE;
//
//     Local<FunctionTemplate> tpl = JS_NEW_FUNCTION_TEMPLATE(_OPjsonCreate);
//     Handle<Object> result = tpl->GetFunction();
//     JS_SET_METHOD(result, "Wrap", _OPjsonWrap);
//     JS_SET_NUMBER(result, "size", sizeof(OPjson));
//     JS_SET_OBJECT(exports, "json", result);
//
// }

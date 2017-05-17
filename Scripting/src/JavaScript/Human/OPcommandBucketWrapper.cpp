// myobject.cc
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"
#include "./Data/include/OPcman.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

// void OPcommandBucketInit(OPcommandBucket* commandBucket, OPuint bucketSize, OPcam* camera);
// void OPcommandBucketInit(OPcommandBucket* commandBucket, OPuint bucketSize, OPcam* camera, OPallocator* allocator);
// OPcommandBucket* OPcommandBucketCreate(OPuint bucketSize);
// OPcommandBucket* OPcommandBucketCreate(OPuint bucketSize, OPallocator* allocator);
// void OPcommandBucketFlush(OPcommandBucket* commandBucket);
// void OPcommandBucketSortKeys(OPcommandBucket* commandBucket);
// void OPcommandBucketSubmit(OPcommandBucket* commandBucket, ui64 key, void(*dispatch)(void*, OPcam*), void* data, void* next);
// inline void OPcommandBucketSubmit(OPcommandBucket* commandBucket, ui64 key, void(*dispatch)(void*, OPcam*), void* data);
// inline void OPcommandBucketRender(OPcommandBucket* commandBucket);

void OPrenderCommandBucketWrapperCreate(Handle<Object> result, OPrenderCommandBucket* cb);

JS_RETURN_VAL _OPcommandBucketCreate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;


    OPcam** camera = JS_GET_ARG_PTR(args, 1, OPcam*);

	OPrenderCommandBucket* cb = OPrenderCommandBucket::Create(args[0]->IntegerValue(), camera);
    Handle<Object> result = JS_NEW_OBJECT();
	OPrenderCommandBucketWrapperCreate(result, cb);

    JS_RETURN(result);
}

JS_RETURN_VAL _OPcommandBucketCreateDrawIndexedSubmit(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

	OPrenderCommandBucket* ptr = JS_GET_ARG_PTR(args, 0, OPrenderCommandBucket);
	OPmodel* model = JS_GET_ARG_PTR(args, 1, OPmodel);
	OPmat4* world = JS_GET_ARG_PTR(args, 2, OPmat4);
    OPmaterial* material = JS_GET_ARG_PTR(args, 3, OPmaterial);

    ptr->Submit(model, world, material);

    JS_RETURN_NULL;
}

// JS_RETURN_VAL _OPcommandBucketSubmit(const JS_ARGS& args) {
//     SCOPE_AND_ISOLATE;
// 	// void OPcommandBucketSubmit(OPcommandBucket* commandBucket,
// 	// ui64 key, void(*dispatch)(void*, OPcam*), void* data, void* next);
//
//     OPcommandBucket* ptr = JS_GET_ARG_PTR(args, 0, OPcommandBucket);
//     ui64 key = args[1]->IntegerValue();
//     void* timer = JS_GET_ARG_PTR(args, 2, void);
//     OPcommandBucketRender(ptr);
//
//     JS_RETURN_NULL;
// }
//
// JS_RETURN_VAL _OPcommandBucketSubmitSelf(const JS_ARGS& args) {
//     SCOPE_AND_ISOLATE;
//
//     OPcommandBucket* ptr = JS_GET_PTR(args.This(), OPcommandBucket);
//     OPcommandBucketRender(ptr);
//
//     JS_RETURN_NULL;
// }

JS_RETURN_VAL _OPcommandBucketFlush(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

	OPrenderCommandBucket* ptr = JS_GET_ARG_PTR(args, 0, OPrenderCommandBucket);
	ptr->Flush();

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPcommandBucketFlushSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

	OPrenderCommandBucket* ptr = JS_GET_PTR(args.This(), OPrenderCommandBucket);
	ptr->Flush();

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPcommandBucketRender(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

	OPrenderCommandBucket* ptr = JS_GET_ARG_PTR(args, 0, OPrenderCommandBucket);
	ptr->Render();

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPcommandBucketRenderSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

	OPrenderCommandBucket* ptr = JS_GET_PTR(args.This(), OPrenderCommandBucket);
	ptr->Render();

    JS_RETURN_NULL;
}

void OPrenderCommandBucketWrapperCreate(Handle<Object> result, OPrenderCommandBucket* cb) {
    SCOPE_AND_ISOLATE

    JS_SET_PTR(result, cb);
    JS_SET_METHOD(result, "Flush", _OPcommandBucketFlushSelf);
    JS_SET_METHOD(result, "Render", _OPcommandBucketRenderSelf);

}

void OPrenderCommandBucketWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_METHOD(result, "Create", _OPcommandBucketCreate);
    JS_SET_METHOD(result, "CreateDrawIndexedSubmit", _OPcommandBucketCreateDrawIndexedSubmit);
    JS_SET_METHOD(result, "Flush", _OPcommandBucketFlush);
    JS_SET_METHOD(result, "Render", _OPcommandBucketRender);
    JS_SET_NUMBER(result, "size", sizeof(OPrenderCommandBucket));
    JS_SET_OBJECT(exports, "commandBucket", result);

}

#endif

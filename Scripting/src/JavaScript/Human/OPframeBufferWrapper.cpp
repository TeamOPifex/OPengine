// myobject.cc
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Human/Human.h"

JS_RETURN_VAL _OPframeBufferCreateDepth(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

	//OPtextureDesc desc = {
	//	4096,
	//	4096,
	//	GL_DEPTH_COMPONENT16,
	//	GL_DEPTH_COMPONENT,
	//	GL_FLOAT,
	//	GL_LINEAR,
	//	GL_LINEAR,
	//	GL_CLAMP_TO_EDGE,
	//	GL_CLAMP_TO_EDGE
	//};
	OPtextureDesc desc;
	OPframeBuffer* fb = (OPframeBuffer*)OPalloc(sizeof(OPframeBuffer));
	*fb = OPframeBufferCreateDepth(desc);

    Handle<Object> frameBuffer = JS_NEW_OBJECT();
	JS_SET_PTR(frameBuffer, fb);

    Handle<Object> texture = JS_NEW_OBJECT();
    JS_SET_PTR(texture, &fb->Texture);
    JS_SET_OBJECT(frameBuffer, "Texture", texture);

    JS_RETURN(frameBuffer);
}

JS_RETURN_VAL _OPframeBufferBind(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

	OPframeBuffer* obj = JS_GET_ARG_PTR(args, 0, OPframeBuffer);
	OPframeBufferBind(obj);

	JS_RETURN_NULL;
}

JS_RETURN_VAL _OPframeBufferUnbind(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPframeBufferUnbind();

	JS_RETURN_NULL;
}

void OPframeBufferWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> frameBuffer = JS_NEW_OBJECT();
	JS_SET_METHOD(frameBuffer, "Bind", _OPframeBufferBind);
	JS_SET_METHOD(frameBuffer, "Unbind", _OPframeBufferUnbind);
	JS_SET_METHOD(frameBuffer, "CreateDepth", _OPframeBufferCreateDepth);
    JS_SET_OBJECT(exports, "frameBuffer", frameBuffer);

}

#endif

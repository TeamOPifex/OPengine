// myobject.cc
#include "./Scripting/include/Node/Human/Wrappers.h"
#include "./Scripting/include/Node/Math/Wrappers.h"

#include "./Pipeline/Pipeline.h"

NODE_RETURN_VAL _OPtexture2DCreate(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPtexture* texture = NODE_GET_ARG_PTR(args, 0, OPtexture);
    OPeffect* effect = NODE_GET_ARG_PTR(args, 1, OPeffect);
    OPtexture2D* tex2d;

    if(args.Length() > 2) {
        tex2d = OPtexture2DCreate(texture, effect, OPvec2Create(args[2]->NumberValue(), args[3]->NumberValue()), OPvec2Create(args[4]->NumberValue(), args[5]->NumberValue()));
    } else {
        tex2d = OPtexture2DCreate(texture, effect);
    }

    Handle<Object> result = NODE_NEW_OBJECT();
    NODE_SET_PTR(result, tex2d);

    Handle<Object> scale = NODE_NEW_OBJECT();
    NODE_SET_PTR(scale, &tex2d->Scale);
    NODE_SET_OBJECT(result, "Scale", scale);

    NODE_RETURN(result);
}

NODE_RETURN_VAL _OPtexture2DDestroy(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPtexture2D* tex2d = NODE_GET_ARG_PTR(args, 0, OPtexture2D);
    OPtexture2DDestroy(tex2d);

    NODE_RETURN_NULL
}

NODE_RETURN_VAL _OPtexture2DPrepRender(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPtexture2D* tex2d = NODE_GET_ARG_PTR(args, 0, OPtexture2D);
    OPtexture2DPrepRender(tex2d);

    NODE_RETURN_NULL
}

NODE_RETURN_VAL _OPtexture2DRender(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPtexture2D* tex2d = NODE_GET_ARG_PTR(args, 0, OPtexture2D);
    OPtexture2DRender(tex2d);

    NODE_RETURN_NULL
}

void OPtexture2DWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> texture2D = NODE_NEW_OBJECT();
    NODE_SET_METHOD(texture2D, "Create", _OPtexture2DCreate);
    NODE_SET_METHOD(texture2D, "Destroy", _OPtexture2DDestroy);
    NODE_SET_METHOD(texture2D, "PrepRender", _OPtexture2DPrepRender);
    NODE_SET_METHOD(texture2D, "Render", _OPtexture2DRender);
    NODE_SET_NUMBER(texture2D, "size", sizeof(OPtexture2D));
    NODE_SET_OBJECT(exports, "texture2D", texture2D);
}
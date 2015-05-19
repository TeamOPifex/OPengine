// myobject.cc
#include "./Scripting/include/Node/Human/Wrappers.h"
#include "./Scripting/include/Node/Math/Wrappers.h"

#include "./Human/Human.h"

// OP.render.Init
NODE_RETURN_VAL _OPrenderInit(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPrenderInit();

    NODE_RETURN_NULL;
}

// OP.render.Clear
NODE_RETURN_VAL _OPrenderClear(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPrenderClear(
    args[0]->NumberValue(),
    args[1]->NumberValue(),
    args[2]->NumberValue());

    NODE_RETURN_NULL;
}

// OP.render.Present
NODE_RETURN_VAL _OPrenderPresent(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPrenderPresent();

    NODE_RETURN_NULL;
}

// OP.render.Size
NODE_RETURN_VAL _OPrenderSize(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

    Handle<Object> obj = NODE_NEW_OBJECT();
    NODE_SET_NUMBER(obj, "Width", OPRENDER_WIDTH);
    NODE_SET_NUMBER(obj, "Height", OPRENDER_HEIGHT);
    NODE_SET_NUMBER(obj, "ScreenWidth", OPRENDER_SCREEN_WIDTH);
    NODE_SET_NUMBER(obj, "ScreenHeight", OPRENDER_SCREEN_HEIGHT);
    NODE_SET_NUMBER(obj, "ScaledWidth", OPRENDER_SCALED_WIDTH);
    NODE_SET_NUMBER(obj, "ScaledHeight", OPRENDER_SCALED_HEIGHT);

    NODE_RETURN(obj);
}
NODE_RETURN_VAL _OPrenderDepth(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPrenderDepth(args[0]->IntegerValue());

    NODE_RETURN_NULL;
}

void OPrenderWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> render = NODE_NEW_OBJECT();
    NODE_SET_METHOD(render, "Init", _OPrenderInit);
    NODE_SET_METHOD(render, "Clear", _OPrenderClear);
    NODE_SET_METHOD(render, "Present", _OPrenderPresent);
    NODE_SET_METHOD(render, "Size", _OPrenderSize);
    NODE_SET_METHOD(render, "Depth", _OPrenderDepth);
    NODE_SET_OBJECT(exports, "render", render);

}
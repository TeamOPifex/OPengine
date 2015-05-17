#ifndef OPENGINE_SCRIPTING_NODE_MATH_OPMAT4
#define OPENGINE_SCRIPTING_NODE_MATH_OPMAT4

#include "./Scripting/include/Node/OPscriptNodeHelper.h"
#include "./Math/include/OPmat4.h"

class OPmat4Wrapper : public node::ObjectWrap {
public:
    static NODE_RETURN_VAL Init(Handle<Object> exports);
    static NODE_RETURN_VAL CreateRotX(const NODE_ARGS& args);
    static NODE_RETURN_VAL CreateRotY(const NODE_ARGS& args);
    static NODE_RETURN_VAL CreateRotZ(const NODE_ARGS& args);
    static NODE_RETURN_VAL CreateTranslate(const NODE_ARGS& args);
    static NODE_RETURN_VAL CreateScale(const NODE_ARGS& args);

    inline OPmat4 value() const { return value_; }
    OPmat4 value_;

private:
    explicit OPmat4Wrapper();
    ~OPmat4Wrapper();

    static NODE_RETURN_VAL New(const NODE_ARGS& args);
    static NODE_RETURN_VAL RotX(const NODE_ARGS& args);
    static NODE_RETURN_VAL RotY(const NODE_ARGS& args);
    static NODE_RETURN_VAL RotZ(const NODE_ARGS& args);
    static NODE_RETURN_VAL Translate(const NODE_ARGS& args);
    static NODE_RETURN_VAL Scale(const NODE_ARGS& args);
    static NODE_RETURN_VAL Log(const NODE_ARGS& args);

    static Persistent<Function> constructor;
};

#endif
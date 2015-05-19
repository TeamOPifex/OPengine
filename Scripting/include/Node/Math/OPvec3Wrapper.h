#ifndef OPENGINE_SCRIPTING_NODE_MATH_OPVEC3
#define OPENGINE_SCRIPTING_NODE_MATH_OPVEC3

#include "./Scripting/include/Node/OPscriptNodeHelper.h"
#include "./Math/include/OPvec3.h"


void OPvec3Wrapper(Handle<Object> exports);

//class OPvec3Wrapper : public node::ObjectWrap {
//public:
//    static NODE_RETURN_VAL Init(Handle<Object> exports);
//    //static NODE_RETURN_VAL CreateRotX(const NODE_ARGS& args);
//
//    inline OPvec3 value() const { return value_; }
//    OPvec3 value_;
//
//    explicit OPvec3Wrapper();
//    ~OPvec3Wrapper();
//
//private:
//
//    static NODE_RETURN_VAL New(const NODE_ARGS& args);
//    //static NODE_RETURN_VAL RotX(const NODE_ARGS& args);
//
//    static Persistent<Function> constructor;
//};

#endif
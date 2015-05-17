#ifndef OPENGINE_SCRIPTING_NODE_HUMAN_OPCAM
#define OPENGINE_SCRIPTING_NODE_HUMAN_OPCAM

#include "./Scripting/include/Node/OPscriptNodeHelper.h"
#include "./Human/include/Rendering/OPcam.h"

class OPcamWrapper : public node::ObjectWrap {
public:
    static NODE_RETURN_VAL Init(Handle<Object> exports);
    static NODE_RETURN_VAL _UpdateView(const NODE_ARGS& args);
    static NODE_RETURN_VAL _UpdateProj(const NODE_ARGS& args);
    OPcam value_;

private:
    explicit OPcamWrapper();
    ~OPcamWrapper();

    static NODE_RETURN_VAL NewPersp(const NODE_ARGS& args);
    static NODE_RETURN_VAL NewOrtho(const NODE_ARGS& args);
    static NODE_RETURN_VAL UpdateView(const NODE_ARGS& args);
    static NODE_RETURN_VAL UpdateProj(const NODE_ARGS& args);

    static Persistent<Function> constructorPersp;
    static Persistent<Function> constructorOrtho;
};

#endif
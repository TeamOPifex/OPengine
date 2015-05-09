#include "./Scripting/include/Node/OPscriptNodeWrapperCore.h"
#include "./Scripting/include/Node/OPscriptNodeWrapperMath.h"
#include "./Scripting/include/Node/OPscriptNodeWrapperHuman.h"

void InitializeMethods(Handle<Object> exports) {
    OPscriptNodeWrapperCore(exports);
    OPscriptNodeWrapperMath(exports);
    OPscriptNodeWrapperHuman(exports);
//	GlobalInitializeMethodsO(NULL, target);
//	MathInitializeMethodsO(NULL, target);
//	DataInitializeMethodsO(NULL, target);
//	HumanInitializeMethodsO(NULL, target);
}

NODE_MODULE(OPengine, InitializeMethods)
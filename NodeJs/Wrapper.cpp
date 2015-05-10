#include "./Scripting/include/Node/OPscriptNodeWrapperCore.h"
#include "./Scripting/include/Node/OPscriptNodeWrapperData.h"
#include "./Scripting/include/Node/OPscriptNodeWrapperMath.h"
#include "./Scripting/include/Node/OPscriptNodeWrapperPerformance.h"
#include "./Scripting/include/Node/OPscriptNodeWrapperHuman.h"
#include "./Scripting/include/Node/OPscriptNodeWrapperCommunication.h"
#include "./Scripting/include/Node/OPscriptNodeWrapperPipeline.h"

void InitializeMethods(Handle<Object> exports) {
    OPscriptNodeWrapperCore(exports);
    OPscriptNodeWrapperData(exports);
    OPscriptNodeWrapperMath(exports);
    OPscriptNodeWrapperPerformance(exports);
    OPscriptNodeWrapperHuman(exports);
    OPscriptNodeWrapperCommunication(exports);
    OPscriptNodeWrapperPipeline(exports);
}

NODE_MODULE(OPengine, InitializeMethods)
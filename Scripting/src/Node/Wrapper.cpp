#ifdef OPIFEX_OPTION_NODEJS

#include "./Scripting/include/Node/Core/Wrappers.h"
#include "./Scripting/include/Node/Data/Wrappers.h"
#include "./Scripting/include/Node/Math/Wrappers.h"
#include "./Scripting/include/Node/Performance/Wrappers.h"
#include "./Scripting/include/Node/Human/Wrappers.h"
#include "./Scripting/include/Node/Communication/Wrappers.h"
#include "./Scripting/include/Node/Pipeline/Wrappers.h"

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

#endif
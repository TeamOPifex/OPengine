#ifdef OPIFEX_OPTION_NODEJS

#include "./Scripting/include/JavaScript/Core/Wrappers.h"
#include "./Scripting/include/JavaScript/Data/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"
#include "./Scripting/include/JavaScript/Performance/Wrappers.h"
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Communication/Wrappers.h"
#include "./Scripting/include/JavaScript/Pipeline/Wrappers.h"

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
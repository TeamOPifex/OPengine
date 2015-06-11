#ifdef OPIFEX_OPTION_NODEJS

#include "./Scripting/include/JavaScript/OPjavaScriptHelper.h"
#include "./Scripting/include/JavaScript/Core/Wrappers.h"
#include "./Scripting/include/JavaScript/Data/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"
#include "./Scripting/include/JavaScript/Performance/Wrappers.h"
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Communication/Wrappers.h"
#include "./Scripting/include/JavaScript/Pipeline/Wrappers.h"

void InitializeMethods(Handle<Object> exports) {

    SCOPE_AND_ISOLATE;

	Handle<Object> OP = JS_NEW_OBJECT();

    OPscriptNodeWrapperCore(OP);
    OPscriptNodeWrapperData(OP);
    OPscriptNodeWrapperMath(OP);
    OPscriptNodeWrapperPerformance(OP);
    OPscriptNodeWrapperHuman(OP);
    OPscriptNodeWrapperCommunication(OP);
    OPscriptNodeWrapperPipeline(OP);

    JS_SET_OBJECT(exports, "OP", OP);
}

NODE_MODULE(OPengine, InitializeMethods)

#endif
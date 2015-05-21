#include "./Scripting/include/JavaScript/OPjavaScriptHelper.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Scripting/include/JavaScript/Math/Wrappers.h"

void OPscriptNodeWrapperMath(Handle<Object> exports) {
    OPmat4Wrapper(exports);
    OPvec2Wrapper(exports);
    OPvec3Wrapper(exports);
}

#endif
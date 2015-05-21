#include "./Scripting/include/JavaScript/OPjavaScriptHelper.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Scripting/include/JavaScript/Data/Wrappers.h"

void OPscriptNodeWrapperData(Handle<Object> exports) {
    OPcmanWrapper(exports);
}

#endif
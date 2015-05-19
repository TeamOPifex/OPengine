#include "./Scripting/include/Node/OPscriptNodeHelper.h"

#ifdef OPIFEX_OPTION_NODEJS

#include "./Scripting/include/Node/Data/Wrappers.h"

void OPscriptNodeWrapperData(Handle<Object> exports) {
    OPcmanWrapper(exports);
}

#endif
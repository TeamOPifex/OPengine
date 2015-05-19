#include "./Scripting/include/Node/OPscriptNodeHelper.h"

#ifdef OPIFEX_OPTION_NODEJS

#include "./Scripting/include/Node/Math/Wrappers.h"

void OPscriptNodeWrapperMath(Handle<Object> exports) {
    OPmat4Wrapper(exports);
    OPvec2Wrapper(exports);
    OPvec3Wrapper(exports);
}

#endif
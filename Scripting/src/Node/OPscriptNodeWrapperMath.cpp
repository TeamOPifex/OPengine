#include "./Scripting/include/Node/OPscriptNodeHelper.h"

#ifdef OPIFEX_OPTION_NODEJS

#include "./Scripting/include/Node/OPscriptNodeWrapperMath.h"
#include "./Scripting/include/Node/Math/Wrappers.h"

void OPscriptNodeWrapperMath(Handle<Object> exports) {
    SCOPE_AND_ISOLATE

    OPmat4Wrapper(exports);
    OPvec2Wrapper(exports);
    OPvec3Wrapper(exports);

}

#endif
#include "./Scripting/include/Node/OPscriptNodeHelper.h"

#ifdef OPIFEX_OPTION_NODEJS

#include "./Scripting/include/Node/OPscriptNodeWrapperMath.h"
#include "./Scripting/include/Node/Math/Wrappers.h"
#include "./Scripting/include/Node/Human/Wrappers.h"

void OPscriptNodeWrapperHuman(Handle<Object> exports) {
    SCOPE_AND_ISOLATE

    OPcamWrapper(exports);
    OPcubeWrapper(exports);
    OPeffectWrapper(exports);
    OPfontWrapper(exports);
    OPgamePadWrapper(exports);
    OPkeyboardWrapper(exports);
    OPmaterialWrapper(exports);
    OPmeshWrapper(exports);
    OPrenderWrapper(exports);
    OPtextureWrapper(exports);

    OPATTRWrapper(exports);
}

#endif
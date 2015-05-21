#include "./Scripting/include/JavaScript/OPjavaScriptHelper.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Scripting/include/JavaScript/Human/Wrappers.h"

void OPscriptNodeWrapperHuman(Handle<Object> exports) {
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
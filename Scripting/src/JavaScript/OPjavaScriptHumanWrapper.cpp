#include "./Scripting/include/JavaScript/OPjavaScriptHelper.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Scripting/include/JavaScript/Human/Wrappers.h"

void OPscriptNodeWrapperHuman(Handle<Object> exports) {
    OPcamFreeFlightWrapper(exports);
    OPcamWrapper(exports);
    OPcubeWrapper(exports);
    OPeffectWrapper(exports);
    OPfontWrapper(exports);
    OPfontManagerWrapper(exports);
    OPfontRenderWrapper(exports);
    OPgamePadWrapper(exports);
    OPkeyboardWrapper(exports);
    OPmaterialWrapper(exports);
    OPmeshWrapper(exports);
    OPmodelWrapper(exports);
    OPrenderWrapper(exports);
    OPskeletonWrapper(exports);
    OPskeletonAnimationWrapper(exports);
    OPtextureWrapper(exports);
    OPframeBufferWrapper(exports);
    OPrenderCommandBucketWrapper(exports);
    OPcommandDrawIndexedWrapper(exports);
    OPvertexLayoutWrapper(exports);

    OPATTRWrapper(exports);
}

#endif

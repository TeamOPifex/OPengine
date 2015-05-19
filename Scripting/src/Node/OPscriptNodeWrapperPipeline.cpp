#include "./Scripting/include/Node/OPscriptNodeHelper.h"

#ifdef OPIFEX_OPTION_NODEJS

#include "./Scripting/include/Node/Pipeline/Wrappers.h"

void OPscriptNodeWrapperPipeline(Handle<Object> exports) {
    OPtexture2DWrapper(exports);
    OPloaderWrapper(exports);
}

#endif
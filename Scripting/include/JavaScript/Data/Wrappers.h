#ifndef OPENGINE_SCRIPTING_NODE_DATA_WRAPPERS
#define OPENGINE_SCRIPTING_NODE_DATA_WRAPPERS

#ifdef OPIFEX_OPTION_NODEJS

#include "./Scripting/include/JavaScript/OPjavaScriptHelper.h"

void OPscriptNodeWrapperData(Handle<Object> target);

void OPcmanWrapper(Handle<Object> exports);

#endif

#endif //OPIFEXENGINE_WRAPPER_H

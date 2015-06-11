#ifndef OPENGINE_SCRIPTING_NODE_COMMUNICATION_WRAPPERS
#define OPENGINE_SCRIPTING_NODE_COMMUNICATION_WRAPPERS

#include "./Scripting/include/JavaScript/OPjavaScriptHelper.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

void OPscriptNodeWrapperCommunication(Handle<Object> target);

#endif

#endif //OPIFEXENGINE_WRAPPER_H

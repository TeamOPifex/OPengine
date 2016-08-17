#pragma once

#include "./Scripting/include/JavaScript/OPjavaScriptHelper.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

void OPscriptNodeWrapperData(Handle<Object> target);

void OPcmanWrapper(Handle<Object> exports);

#endif
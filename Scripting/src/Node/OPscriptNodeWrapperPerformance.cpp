#include "./Scripting/include/Node/OPscriptNodeHelper.h"

#ifdef OPIFEX_OPTION_NODEJS

#include "./Scripting/include/Node/OPscriptNodeWrapperPerformance.h"

#include "./Performance/Performance.h"

void OPscriptNodeWrapperPerformance(Handle<Object> exports) {
    SCOPE_AND_ISOLATE

    Handle<Object> performance = Object::New(isolate);

    exports->Set(String::NewFromUtf8(isolate, "performance"), performance);
}

#endif
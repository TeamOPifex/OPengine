#include "./Scripting/include/Node/OPscriptNodeHelper.h"

#ifdef OPIFEX_OPTION_NODEJS

#include "./Scripting/include/Node/OPscriptNodeWrapperCommunication.h"

#include "./Communication/Communication.h"

void OPscriptNodeWrapperCommunication(Handle<Object> exports) {
    SCOPE_AND_ISOLATE

    Handle<Object> communication = Object::New(isolate);

    exports->Set(String::NewFromUtf8(isolate, "communication"), communication);
}

#endif
#include "./Scripting/include/Node/OPscriptNodeHelper.h"

#ifdef OPIFEX_OPTION_NODEJS

#include "./Scripting/include/Node/OPscriptNodeWrapperCommunication.h"

#include "./Communication/Communication.h"

void OPscriptNodeWrapperCommunication(Handle<Object> exports) {
    SCOPE_AND_ISOLATE

    Handle<Object> communication = NODE_NEW_OBJECT();
    NODE_SET_OBJECT(exports, "communication", communication);

}

#endif
#ifndef OP_ENGINE_CORE_DIR
#define OP_ENGINE_CORE_DIR

#include "./Core/include/OPtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

	OPchar* OPdirCurrent();
	OPchar* OPdirExecutable();

#ifdef __cplusplus
}
#endif


#endif
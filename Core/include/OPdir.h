#ifndef OP_ENGINE_CORE_DIR
#define OP_ENGINE_CORE_DIR

#include "./Core/include/OPtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

	EXPORT_METHOD OPchar* OPdirCurrent();
	EXPORT_METHOD OPchar* OPdirExecutable();

#ifdef __cplusplus
}
#endif


#endif
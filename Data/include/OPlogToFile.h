#ifndef OPENGINE_DATA_LOG
#define OPENGINE_DATA_LOG

#include "./Core/include/OPlog.h"

#ifdef __cplusplus
extern "C" {
#endif

	void OPlogToFile(const char* path);
	void OPlogToFileClose();

#ifdef __cplusplus
};
#endif

#endif
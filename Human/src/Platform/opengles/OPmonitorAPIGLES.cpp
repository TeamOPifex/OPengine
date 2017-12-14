#include "./Human/include/Platform/opengles/OPmonitorAPIGLES.h"

#ifdef OPIFEX_OPENGL_ES_2

#include "./Human/include/Rendering/OPmonitor.h"
#include "./Core/include/OPmemory.h"

OPmonitorResult OPmonitorGLESGetAll() {
	OPmonitorResult result;

	result.count = 0;
	result.monitors = NULL;

	return result;
}

void OPmonitorAPIGLESInit(OPmonitorAPI* monitor) {
	monitor->GetAll = OPmonitorGLESGetAll;
}
#endif
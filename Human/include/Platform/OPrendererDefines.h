#pragma once

#include "./Core/include/OPtypes.h"

#if defined(OPIFEX_OPENGL_2_0) || defined(OPIFEX_OPENGL_3_3)
	#define OPIFEX_OPENGL 1
#endif

#if defined(OPIFEX_OPENGL_ES_2)
#define OPIFEX_OPENGL_ES 1
#endif
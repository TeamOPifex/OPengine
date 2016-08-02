#include "./Human/include/Rendering/OPrender.h"

OPrenderer* OPRENDERER_ACTIVE = NULL;

#if defined(OPIFEX_OPENGL_3_3)
#define OPIFEX_OPENGL 1
#define OPIFEX_OPENGL_MAJOR 3
#define OPIFEX_OPENGL_MINOR 3
#endif

#ifdef OPIFEX_OPENGL

#include "./Human/include/Input/OPkeyboard.h"
#include "./Core/include/Assert.h"
#include "include/Utilities/Errors.h"
#include "./Human/include/Rendering/OPmonitor.h"
#include "./Human/include/Rendering/OPwindow.h"
#include "./Human/include/Platform/opengl/OPrendererGL.h"

ui32 OPRENDER_WIDTH = 1280;
ui32 OPRENDER_HEIGHT = 720;
ui32 OPRENDER_SCREEN_WIDTH = 1280;
ui32 OPRENDER_SCREEN_HEIGHT = 720;
ui32 OPRENDER_SCALED_WIDTH = 1280;
ui32 OPRENDER_SCALED_HEIGHT = 720;
f32 OPRENDER_SCREEN_WIDTH_SCALE = 1;
f32 OPRENDER_SCREEN_HEIGHT_SCALE = 1;
i8 OPRENDER_FULLSCREEN = false;
i8 OPRENDER_HAS_FOCUS = 1;
OPuint OPRENDER_VAO = 0;


OPint OPrenderInit() {
#ifdef OPIFEX_OPENGL
	OPRENDERER_ACTIVE = OPrendererGL();
	OPRENDERER_ACTIVE->Init();
#else
#endif
	return 0;
}

OPint OPrenderInit(OPrendererType renderer) {
	switch (renderer) {
		case OPrendererType::OPENGL: {
			OPRENDERER_ACTIVE = OPrendererGL();
			OPRENDERER_ACTIVE->Init();
			break;
		}
	}

    return 0;
}



void OPrenderResetViewport(OPuint x, OPuint y, OPuint width, OPuint height) {
	OPRENDERER_ACTIVE->SetViewport(0, 0, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height);
}


//void  OPrenderSetScreenSize(OPuint width, OPuint height) {
//    OPRENDER_SCREEN_WIDTH = width;
//    OPRENDER_SCREEN_HEIGHT = height;
//}


OPint OPrenderGetWidth() {
    return OPRENDER_WIDTH;
}

OPint OPrenderGetHeight() {
    return OPRENDER_HEIGHT;
}

OPfloat OPrenderGetAspectRatio() {
    ASSERT(OPrenderGetHeight() > 0, "Height was not greater than 0, there was problem getting width and height");
    return OPrenderGetWidth() / (OPfloat)OPrenderGetHeight();
}

OPfloat OPrenderGetWidthAspectRatio() {
    ASSERT(OPRENDER_WIDTH > 0, "Height was not greater than 0, there was problem getting width and height");
    OPfloat aspect = OPRENDER_HEIGHT / (OPfloat)OPRENDER_WIDTH;
    return aspect > 1.0f ? 1.0f : aspect;
}

OPfloat OPrenderGetHeightAspectRatio() {
    ASSERT(OPRENDER_HEIGHT > 0, "Height was not greater than 0, there was problem getting width and height");
    OPfloat aspect = OPRENDER_WIDTH / (OPfloat)OPRENDER_HEIGHT;
    return aspect > 1.0f ? 1.0f : aspect;
}


// ui32 OPgetNativeScreenWidth() {
//     if (!glfwInitialized) {
//         int result = glfwInit();
//         if (!result) {
//             OPlogErr("INIT FAILED %d", result);
//             return -1;
//         }
//         glfwInitialized = 1;
//     }
//     return glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
// }
// ui32 OPgetNativeScreenHeight() {
//     if (!glfwInitialized) {
//         int result = glfwInit();
//         if (!result) {
//             OPlogErr("INIT FAILED %d", result);
//             return -1;
//         }
//         glfwInitialized = 1;
//     }
//     return glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
// }
#endif

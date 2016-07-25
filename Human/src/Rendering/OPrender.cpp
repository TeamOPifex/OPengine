#include "./Human/include/Rendering/OPrender.h"

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

OPrenderer* OPRENDERER_ACTIVE = NULL;

OPint OPrenderInit(OPwindow* window) {
#ifdef OPIFEX_OPENGL
	OPRENDERER_ACTIVE = OPrendererGL();
	OPRENDERER_ACTIVE->Init(window);
#else
#endif
	return 0;
}

OPint OPrenderInit(OPwindow* window, OPRENDERER renderer) {
	switch (renderer) {
		case OPRENDERER_OPENGL: {
			OPRENDERER_ACTIVE = OPrendererGL();
			OPRENDERER_ACTIVE->Init(window);
			break;
		}
	}

    return 0;
}

void  OPrenderClear(f32 r, f32 g, f32 b, f32 a) {
	OPRENDERER_ACTIVE->Clear(r, g, b, a);
}

void OPrenderCull(bool state) {
	OPRENDERER_ACTIVE->SetCull(state);
}

void OPrenderCullMode(i8 state) {
	OPRENDERER_ACTIVE->SetCullMode(state);
}

void  OPrenderSwapBuffer() {
	OPRENDERER_ACTIVE->SwapBuffer();
}

void OPrenderPresent() {
	OPRENDERER_ACTIVE->SwapBuffer();
}

void OPrenderBlend(bool state) {
	OPRENDERER_ACTIVE->SetBlend(state);
}

void OPrenderDepth(bool state) {
	OPRENDERER_ACTIVE->SetDepthTesting(state);
}

void OPrenderDepthWrite(bool state) {
	OPRENDERER_ACTIVE->SetDepthWrite(state);
}

void OPrenderShutdown() {
	OPRENDERER_ACTIVE->Shutdown();
}




void  OPrenderSetScreenSize(OPuint width, OPuint height) {
    OPRENDER_SCREEN_WIDTH = width;
    OPRENDER_SCREEN_HEIGHT = height;
}

void  OPrenderSetViewport(OPint x, OPint y, ui32 width, ui32 height) {
    glViewport(x, y, width, height);
    OPRENDER_WIDTH  = width;
    OPRENDER_HEIGHT = height;
}

void OPrenderResetViewport() {
    OPrenderSetViewport(0, 0, OPRENDER_SCREEN_WIDTH, OPRENDER_SCREEN_HEIGHT);
}

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

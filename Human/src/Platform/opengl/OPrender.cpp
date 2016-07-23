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

i8 OPRENDER_INITIALIZED = 0;
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

ui8 glfwInitialized = 0;


void glfwErrorCallback(int error, const char* desc){
    OPlogErr("GLFW ERROR: %s", desc);
}

OPint OPrenderInit() {
    ASSERT(glfwInitialized == 0, "GLFW has already been initialized");

    OPlogInfo("Initializing OpenGL Renderer");

    glfwSetErrorCallback(glfwErrorCallback);

    OPint result = glfwInit();
    if(!result) {
        OPlogErr("Failed to initialize GLFW");
        return -1;
    }
    glfwInitialized = 1;

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPIFEX_OPENGL_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPIFEX_OPENGL_MINOR);

    #ifdef OPIFEX_OSX
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif

    // Get all of the monitors
	int monitorCount;
	GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
	OPMONITOR_COUNT = monitorCount;
	for (ui8 i = 0; i < monitorCount & i < OPMONITOR_MAX; i++) {
		OPMONITOR_LIST[i].Init(monitors[i]);
	}

	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	OPMONITOR_PRIMARY.Init(primaryMonitor);

    // GLFW info
    int major, minor, rev;
    glfwGetVersion(&major, &minor, &rev);
    OPlogInfo("GLFW version %d.%d.%d", major, minor, rev);

	OPRENDER_INITIALIZED = 1;

    return 0;
}

OPint OPrenderInitDevice(OPwindow* window) {
	return 1;
}

// OPwindow* OPrenderCreateWindow(OPmonitor* monitor, bool fullscreen, bool borderless, const OPchar* title, ui32 width, ui32 height) {
// 	OPwindow* window = (OPwindow*)OPalloc(sizeof(OPwindow));
// 	window->Init(monitor, fullscreen, borderless, title, width, height);
// 	return window;
// }

void  OPrenderClear(f32 r, f32 g, f32 b, f32 a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

void OPrenderCull(OPint state) {
    if (state) {
        glEnable(GL_CULL_FACE);
    }
    else {
        glDisable(GL_CULL_FACE);
    }
}
void OPrenderCullMode(OPint state) {
    if (state) {
        glCullFace(GL_FRONT);
    }
    else {
        glCullFace(GL_BACK);
    }
}
void  OPrenderSwapBuffer () {
	ASSERT(OPWINDOW_ACTIVE != NULL, "There must be an active window");
    glfwSwapBuffers(OPWINDOW_ACTIVE->Window);
}

void OPrenderUpdate() {
	ASSERT(OPWINDOW_ACTIVE != NULL, "There must be an active window");
#ifdef OPIFEX_OPENGL_ES_2

#else
	glfwPollEvents();
	if(glfwWindowShouldClose(OPWINDOW_ACTIVE->Window)){
		OPend();
	}
#endif
}

void OPrenderPresent() {
	ASSERT(OPWINDOW_ACTIVE != NULL, "There must be an active window");
    glfwSwapBuffers(OPWINDOW_ACTIVE->Window);
}

void OPrenderBlend(OPint state) {
    if (state)
        glEnable(GL_BLEND);
    else
        glDisable(GL_BLEND);
}

void OPrenderDepth(OPint state) {
    if(state)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}

void OPrenderDepthWrite(OPint state) {
    glDepthMask(state ? GL_TRUE : GL_FALSE);
}

void OPrenderShutdown() {
    glfwTerminate();
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

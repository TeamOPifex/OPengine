
#include "./Human/include/Rendering/OPrender.h"

#if defined(OPIFEX_OPENGL_3_3) && !defined(OPIFEX_ANDROID)

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

GLFWwindow* window;
ui8 glfwInitialized = 0;



void glfwErrorCallback(int error, const char* desc){
    OPlog("GLFW ERROR: %s", desc);
}

void glfwWindowFocusCallback(GLFWwindow* window, int code) {
    OPlogInfo("Focus Result: %d", code);
    OPRENDER_HAS_FOCUS = code;
}
void glfwWindowDropCallback(GLFWwindow* window, int count, const OPchar** files) {
    OPlog("Total Files: %d", count);
    for(OPint i = 0; i < count; i++) {
        OPlog("File: %s", files[i]);
    }
}
void glfwCharacterCallback(GLFWwindow* window, unsigned int codepoint)
{
    OPkeyboardKey(codepoint);
}

void(*OP_WINDOW_DROP)(int, const OPchar**) = NULL;
void OPrenderDragAndDropCB(void(*cb)(int, const OPchar**)) {
	OP_WINDOW_DROP = cb;
}

OPint OPrenderInit() {
    OPlogDebug("Initializing OPENGL_3_3 Renderer");

    glfwSetErrorCallback(glfwErrorCallback);

    ASSERT(glfwInitialized == 0, "GLFW has already been initialized");
    OPint result = glfwInit();
    if(!result) {
        OPlogErr("Failed to initialize GLFW");
        return -1;
    }
    glfwInitialized = 1;


    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    #ifdef OPIFEX_OSX
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif

	int monitorCount;
	GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
	for (ui8 i = 0; i < monitorCount & i < OPMONITOR_MAX; i++) {
		OPMONITOR_LIST[i].Init(monitors[i]);
	}

	OPMONITOR_COUNT = monitorCount;

	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	OPMONITOR_PRIMARY.Init(primaryMonitor);

  //  GLFWmonitor* monitor = NULL;
  //  if (OPRENDER_FULLSCREEN){
		//monitor = primaryMonitor;
  //  }



    //OPint _screenWidth = OPRENDER_SCREEN_WIDTH;
    //OPint _screenHeight = OPRENDER_SCREEN_HEIGHT;

    //OPlog("%d x %d", OPRENDER_SCREEN_WIDTH, OPRENDER_SCREEN_HEIGHT);

    //OPlog("%d x %d", _screenWidth, _screenHeight);





    //window = glfwCreateWindow(_screenWidth, _screenHeight,
    //                          "OPifex Entertainment", monitor, NULL);

    //OPlog("%d x %d", _screenWidth, _screenHeight);

    //OPlogInfo("Created window of size: %d x %d",
    //          _screenWidth, _screenHeight);


    int major, minor, rev;
    glfwGetVersion(&major, &minor, &rev);

    OPlog("GLFW version %d.%d.%d", major, minor, rev);

	//int w, h;
	//glfwGetFramebufferSize(window, &w, &h);
	//OPRENDER_SCREEN_WIDTH = (ui32)w;
	//OPRENDER_SCREEN_HEIGHT = (ui32)h;

 //   OPRENDER_SCREEN_WIDTH_SCALE = _screenWidth / (f32)OPRENDER_SCREEN_WIDTH;
 //   OPRENDER_SCREEN_HEIGHT_SCALE = _screenHeight / (f32)OPRENDER_SCREEN_HEIGHT;
 //   OPRENDER_SCALED_WIDTH = OPRENDER_SCREEN_WIDTH * OPRENDER_SCREEN_WIDTH_SCALE;
 //   OPRENDER_SCALED_HEIGHT = OPRENDER_SCREEN_HEIGHT * OPRENDER_SCREEN_HEIGHT_SCALE;
 //   OPlogInfo("Frame Buffer size: %d x %d", OPRENDER_SCREEN_WIDTH, OPRENDER_SCREEN_HEIGHT);
 //   OPlogDebug("Scale: %f x %f", OPRENDER_SCREEN_WIDTH_SCALE, OPRENDER_SCREEN_HEIGHT_SCALE);

 //   if(!window) {
 //       OPlogErr("Failed to open GLFW window of %dx%d. If you have an Intel GPU, they are not 3.3 compatible.\n", OPRENDER_WIDTH, OPRENDER_HEIGHT );
 //       glfwTerminate();
 //       return -1;
 //   }

	OPRENDER_INITIALIZED = 1;
/*
    glfwMakeContextCurrent(window);
    OPglError("OPrenderInit error glfw current context window");

    glfwSetWindowFocusCallback(window, glfwWindowFocusCallback);
*/
    //OPrenderSetViewport(0, 0, (OPuint)OPRENDER_SCREEN_WIDTH, (OPuint)OPRENDER_SCREEN_HEIGHT);
    //OPglError("OPrenderInit error viewport set");

	//glewExperimental = GL_TRUE;
	//if (glewInit() != GLEW_OK) {
	//	OPlogErr("Failed to init glew");
	//	return -1;
	//}

 //   OPglError("OPrenderInit error glew initialed");

    //glfwSetInputMode(window, GLFW_STICKY_KEYS, true);

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glfwSetCharCallback(window, glfwCharacterCallback);
    //glfwSetDropCallback(window, glfwWindowDropCallback);

    //OPglError("OPrenderInit error glfw setup");


    //GLuint temp;
    //glGenVertexArrays(1, &temp);
    //OPRENDER_VAO = temp;
    //glBindVertexArray(OPRENDER_VAO);

    //OPglError("OPrenderInit error VAO");

    //glEnable(GL_MULTISAMPLE_ARB);
    //glEnable(GL_BLEND);
    //glEnable(GL_MULTISAMPLE);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //OPRENDER_WIDTH = OPRENDER_SCREEN_WIDTH;
    //OPRENDER_HEIGHT = OPRENDER_SCREEN_HEIGHT;

    //OPlogInfo("OpenGL Context Created W:%d H:%d", OPRENDER_WIDTH, OPRENDER_HEIGHT);

    //OPglError("OPrenderInit error clear end");
    return 0;
}

OPint OPrenderInitDevice(OPwindow* window) {
	return 1;
}

OPwindow* OPrenderCreateWindow(OPmonitor* monitor, bool fullscreen, bool borderless, const OPchar* title, ui32 width, ui32 height) {
	OPwindow* window = (OPwindow*)OPalloc(sizeof(OPwindow));
	window->Init(monitor, fullscreen, borderless, title, width, height);
	return window;
}

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

void  OPrenderPresent    () {
	ASSERT(OPWINDOW_ACTIVE != NULL, "There must be an active window");
    glfwSwapBuffers(OPWINDOW_ACTIVE->Window);
}
void OPrenderBlend(OPint state) {
    if (state)
        glEnable(GL_BLEND);
    else
        glDisable(GL_BLEND);
}
void  OPrenderDepth(OPint state) {
    if(state)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}
void OPrenderDepthWrite(OPint state) {
    if (state)
        glDepthMask(GL_TRUE);
    else
        glDepthMask(GL_FALSE);
}
void  OPrenderShutdown   () {
    glfwTerminate();
}

ui32 OPgetNativeScreenWidth() {
    if (!glfwInitialized) {
        int result = glfwInit();
        if (!result) {
            OPlogErr("INIT FAILED %d", result);
            return -1;
        }
        glfwInitialized = 1;
    }
    return glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
}
ui32 OPgetNativeScreenHeight() {
    if (!glfwInitialized) {
        int result = glfwInit();
        if (!result) {
            OPlogErr("INIT FAILED %d", result);
            return -1;
        }
        glfwInitialized = 1;
    }
    return glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
}
#endif

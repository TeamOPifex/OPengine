
#include "./Human/include/Rendering/OPrender.h"

#if defined(OPIFEX_OPENGL_3_3) && !defined(OPIFEX_ANDROID)

#include "./Human/include/Input/OPkeyboard.h"
#include "./Core/include/Assert.h"
#include "include/Utilities/Errors.h"

i32 OPRENDER_WIDTH = 1280;
i32 OPRENDER_HEIGHT = 720;
i32 OPRENDER_SCREEN_WIDTH;
i32 OPRENDER_SCREEN_HEIGHT;
i32 OPRENDER_SCALED_WIDTH;
i32 OPRENDER_SCALED_HEIGHT;
f32 OPRENDER_SCREEN_WIDTH_SCALE;
f32 OPRENDER_SCREEN_HEIGHT_SCALE;
OPint OPRENDER_FULLSCREEN;
OPint OPRENDER_HAS_FOCUS;

GLFWwindow* window;
ui8 glfwInitialized = 0;



void glfwErrorCallback(int error, const char* desc){
    OPlog(desc);
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


OPint OPrenderInit(i32 width, i32 height) {
    OPlogDebug("Initializing Renderer");


    OPRENDER_SCREEN_WIDTH = width;
    OPRENDER_SCREEN_HEIGHT = height;

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
    glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor = NULL;
    if (OPRENDER_FULLSCREEN){
        monitor = glfwGetPrimaryMonitor();
    }

    OPint _screenWidth = OPRENDER_SCREEN_WIDTH;
    OPint _screenHeight = OPRENDER_SCREEN_HEIGHT;

    OPlog("%d x %d", OPRENDER_SCREEN_WIDTH, OPRENDER_SCREEN_HEIGHT);

    OPlog("%d x %d", _screenWidth, _screenHeight);

    int major, minor, rev;
    glfwGetVersion(&major, &minor, &rev);
    OPlog("OpenGL version set %d.%d.%d", major, minor, rev);

    window = glfwCreateWindow(_screenWidth, _screenHeight,
                              "OPifex Entertainment", monitor, NULL);

    OPlog("%d x %d", _screenWidth, _screenHeight);

    OPlogInfo("Created window of size: %d x %d",
              _screenWidth, _screenHeight);

    glfwGetFramebufferSize(window, &OPRENDER_SCREEN_WIDTH, &OPRENDER_SCREEN_HEIGHT);

    OPRENDER_SCREEN_WIDTH_SCALE = _screenWidth / (f32)OPRENDER_SCREEN_WIDTH;
    OPRENDER_SCREEN_HEIGHT_SCALE = _screenHeight / (f32)OPRENDER_SCREEN_HEIGHT;
    OPRENDER_SCALED_WIDTH = OPRENDER_SCREEN_WIDTH * OPRENDER_SCREEN_WIDTH_SCALE;
    OPRENDER_SCALED_HEIGHT = OPRENDER_SCREEN_HEIGHT * OPRENDER_SCREEN_HEIGHT_SCALE;
    OPlogInfo("Frame Buffer size: %d x %d", OPRENDER_SCREEN_WIDTH, OPRENDER_SCREEN_HEIGHT);
    OPlogDebug("Scale: %f x %f", OPRENDER_SCREEN_WIDTH_SCALE, OPRENDER_SCREEN_HEIGHT_SCALE);

    if(!window) {
        OPlogErr("Failed to open GLFW window of %dx%d. If you have an Intel GPU, they are not 3.3 compatible.\n", OPRENDER_WIDTH, OPRENDER_HEIGHT );
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    OPglError("OPrenderInit error glfw current context window");


    glfwSetWindowFocusCallback(window, glfwWindowFocusCallback);

    glewExperimental = GL_TRUE;
    OPrenderSetViewport(0, 0, (OPuint)OPRENDER_SCREEN_WIDTH, (OPuint)OPRENDER_SCREEN_HEIGHT);
    OPglError("OPrenderInit error viewport set");

    if (glewInit() != GLEW_OK) return -1;

    OPglError("OPrenderInit error glew initialed");

    glfwSetInputMode(window, GLFW_STICKY_KEYS, true);

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCharCallback(window, glfwCharacterCallback);
    glfwSetDropCallback(window, glfwWindowDropCallback);

    OPglError("OPrenderInit error glfw setup");

    GLuint VertexArrayID[1];
    glGenVertexArrays(1, VertexArrayID);
    glBindVertexArray(VertexArrayID[0]);

    OPglError("OPrenderInit error VAO");

    glEnable(GL_MULTISAMPLE_ARB);
    glEnable(GL_BLEND);
    glEnable(GL_MULTISAMPLE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    OPRENDER_WIDTH = OPRENDER_SCREEN_WIDTH;
    OPRENDER_HEIGHT = OPRENDER_SCREEN_HEIGHT;

    OPlogInfo("OpenGL Context Created W:%d H:%d", OPRENDER_WIDTH, OPRENDER_HEIGHT);

    OPglError("OPrenderInit error clear end");
    return 0;
}

void  OPrenderClear(f32 r, f32 g, f32 b, f32 a) {

}
void  OPrenderSetScreenSize(OPuint width, OPuint height) {

}
void  OPrenderSetViewport(OPuint x, OPuint y, OPuint width, OPuint height) {

}
void OPrenderResetViewport() {

}
OPint OPrenderGetWidth() {
    return 0;
}
OPint OPrenderGetHeight() {
    return 0;
}
OPfloat OPrenderGetAspectRatio() {
    return 0;
}
OPfloat OPrenderGetWidthAspectRatio() {
    return 0;
}
OPfloat OPrenderGetHeightAspectRatio() {
    return 0;
}

void OPrenderCull(OPint state) {

}
void OPrenderCullMode(OPint state) {

}
void  OPrenderSwapBuffer () {

}
void  OPrenderPresent    () {

}
void OPrenderBlend(OPint state) {

}
void  OPrenderDepth(OPint state) {

}
void OPrenderDepthWrite(OPint state) {

}
void  OPrenderShutdown   () {

}

ui32 OPgetNativeScreenWidth() {

}
ui32 OPgetNativeScreenHeight() {

}
#endif

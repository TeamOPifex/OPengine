#include "./Human/include/Rendering/OPrender.h"

#if defined(OPIFEX_OPENGL_2_0) && !defined(OPIFEX_ANDROID)

#include "./Core/include/OPcore.h"

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
i8 glfwInitialized = 0;
OPuint OPRENDER_VAO = 0;

#ifndef OPIFEX_ANDROID
GLFWwindow* window = NULL;
#else

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/native_window.h>

#include "./Human/include/Utilities/Errors.h"


EGLSurface surface;
EGLDisplay display;

#endif

ui32 OPgetNativeScreenWidth() {
#ifndef OPIFEX_ANDROID
	if (!glfwInitialized) {
		int result = glfwInit();
		if (!result) {
			OPlogErr("INIT FAILED %d", result);
			return -1;
		}
		glfwInitialized = 1;
	}
	return glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
#else
	return JNIWidth();
#endif
}

ui32 OPgetNativeScreenHeight() {
#ifndef OPIFEX_ANDROID
	if (!glfwInitialized) {
		int result = glfwInit();
		if (!result) {
			OPlogErr("INIT FAILED %d", result);
			return -1;
		}
		glfwInitialized = 1;
	}
	return glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
#else
	return JNIHeight();
#endif
}

#ifndef OPIFEX_ANDROID

void(*OP_WINDOW_DROP)(int, const OPchar**) = NULL;

void OPrenderDragAndDropCB(void (*cb)(int, const OPchar**)) {
	OP_WINDOW_DROP = cb;
}

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

	if(OP_WINDOW_DROP != NULL) {
		OP_WINDOW_DROP(count, files);
	}
}
#include "./Human/include/Input/OPkeyboard.h"
void glfwCharacterCallback(GLFWwindow* window, unsigned int codepoint)
{
	OPkeyboardKey(codepoint);
}
#endif

OPint OPrenderInit(){
	OPlogDebug("Initializing OPENGL_2_0 Renderer");

#ifdef OPIFEX_ANDROID
	OPRENDER_SCREEN_WIDTH = JNIWidth();
	OPRENDER_SCREEN_HEIGHT = JNIHeight();

	const EGLint attribs[] = {
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_NONE
	};

	EGLint attribList[] =
	{
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};


	EGLint w, h, dummy, format;
	EGLint numConfigs;
	EGLConfig config;
	EGLContext context;

	OPlogDebug("Getting Display");
	display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	OPlogDebug("Binding API");
	if (eglBindAPI(EGL_OPENGL_ES_API) == GL_FALSE) {
		OPlogErr("FAILED TO BIND ES API");
	}

	OPlogDebug("eglInitialize");
	eglInitialize(display, 0, 0);


	OPlogDebug("eglMakeCurrent");
	eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

	OPlogDebug("eglChooseConfig");
	/* Here, the application chooses the configuration it desires. In this
	* sample, we have a very simplified selection process, where we pick
	* the first EGLConfig that matches our criteria */
	if (eglChooseConfig(display, attribs, &config, 1, &numConfigs) == EGL_FALSE) {
		OPlog("Choose Config Failed");
	}


	OPlogDebug("eglGetConfigAttrib");
	/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
	* guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
	* As soon as we picked a EGLConfig, we can safely reconfigure the
	* ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);


	OPlogDebug("eglCreateWindowSurface");
	surface = eglCreateWindowSurface(display, config, OPAndroidState->window, NULL);


	OPglError("OPrenderInit:Error 2");

	OPlogDebug("eglCreateContext");
	context = eglCreateContext(display, config, NULL, attribList);

	OPglError("OPrenderInit:Error 3");

	OPlog("Android State Window %d", OPAndroidState->window);


	OPlog("eglMakeCurrent");
	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		OPlog("Unable to eglMakeCurrent");
		return -1;
	}

	OPlog("ANativeWindow_setBuffersGeometry");
	ANativeWindow_setBuffersGeometry(OPAndroidState->window, 0, 0, format);

	OPlog("eglQuerySurface");
	OPglError("OPrenderInit:Error 4");
	eglQuerySurface(display, surface, EGL_WIDTH, &w);
	eglQuerySurface(display, surface, EGL_HEIGHT, &h);

	OPRENDER_SCREEN_WIDTH = w;
	OPRENDER_SCREEN_HEIGHT = h;

	OPRENDER_WIDTH = OPRENDER_SCREEN_WIDTH;
	OPRENDER_HEIGHT = OPRENDER_SCREEN_HEIGHT;

	OPglError("OPrenderInit:Error 5");
	//engine->display = display;
	//engine->context = context;
	//engine->surface = surface;
	//engine->width = w;
	//engine->height = h;
	//engine->state.angle = 0;

	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Initialize GL state.
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	//glEnable(GL_CULL_FACE);
	//glShadeModel(GL_SMOOTH);
	//glDisable(GL_DEPTH_TEST);
#endif

#ifdef OPIFEX_OPENGL_ES_2
	// Android doesn't need to create a window
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	////glCullFace(GL_FRONT);
	////glEnable(GL_CULL_FACE);
	//glDisable(GL_CULL_FACE);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable( GL_BLEND );

	//OPrenderWidth = JNIWidth();
	//OPrenderHeight = JNIHeight();
	//OPscreenWidth = JNIWidth();
	//OPscreenHeight = JNIHeight();
#else

	// OPstream* str = OPreadFile("../app.config");
	// if (str) {
	// 	OPrenderFullscreen = OPstreamI32(str);
	// 	OPscreenWidth = OPstreamI32(str);
	// 	if (OPscreenWidth == -1) OPscreenWidth = OPgetNativeScreenWidth();
	// 	OPscreenHeight = OPstreamI32(str);
	// 	if (OPscreenHeight == -1) OPscreenHeight = OPgetNativeScreenHeight();
	// }

	glfwSetErrorCallback(glfwErrorCallback);

	if (!glfwInitialized) {
		glfwInitialized = 1;
		int result = glfwInit();
		if (!result) {
			OPlogErr("INIT FAILED %d", result);
			return -1;
		}
	}

	// Most of the below will be moved to a Windowing System
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);


	int monitorCount;
	GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
	for (ui8 i = 0; i < monitorCount & i < OPMONITOR_MAX; i++) {
		OPMONITOR_LIST[i].Init(monitors[i]);
	}

	OPMONITOR_COUNT = monitorCount;

	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	OPMONITOR_PRIMARY.Init(primaryMonitor);



	int major, minor, rev;
	glfwGetVersion(&major, &minor, &rev);

	OPlog("GLFW version %d.%d.%d", major, minor, rev);

#endif
	OPRENDER_INITIALIZED = 1;
	OPlogInfo("OpenGL Context Created W:%d H:%d", OPRENDER_WIDTH, OPRENDER_HEIGHT);
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

void  OPrenderSetScreenSize(ui32 width, ui32 height) {
	OPRENDER_SCREEN_WIDTH = width;
	OPRENDER_SCREEN_HEIGHT = height;
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
//-----------------------------------------------------------------------------
void  OPrenderClear(f32 r, f32 g, f32 b, f32 a){
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//-----------------------------------------------------------------------------
void  OPrenderSetViewport(OPint x, OPint y, ui32 width, ui32 height){
	glViewport((GLint)x, (GLint)y, width, height);
	OPRENDER_WIDTH  = width;
	OPRENDER_HEIGHT = height;
}

void OPrenderResetViewport() {
	OPrenderSetViewport(0, 0, OPRENDER_SCREEN_WIDTH, OPRENDER_SCREEN_HEIGHT);
}

//-----------------------------------------------------------------------------
OPint OPrenderGetWidth(){
#ifndef OPIFEX_ANDROID
	//glfwGetWindowSize(window, &OPrenderWidth, &OPrenderHeight);
#endif
	return OPRENDER_WIDTH;
}
//-----------------------------------------------------------------------------
OPint OPrenderGetHeight(){
#ifndef OPIFEX_ANDROID
	//glfwGetWindowSize(window, &OPrenderWidth, &OPrenderHeight);
#endif
	return OPRENDER_HEIGHT;
}
//-----------------------------------------------------------------------------
OPfloat OPrenderGetAspectRatio(){
	ASSERT(OPrenderGetHeight() > 0, "Height was not greater than 0, there was problem getting width and height");
	return OPrenderGetWidth() / (OPfloat)OPrenderGetHeight();
}

OPfloat aspect;
//-----------------------------------------------------------------------------
OPfloat OPrenderGetWidthAspectRatio(){
	ASSERT(OPRENDER_WIDTH > 0, "Height was not greater than 0, there was problem getting width and height");
	aspect = OPRENDER_HEIGHT / (OPfloat)OPRENDER_WIDTH;
	return aspect > 1.0f ? 1.0f : aspect;
}
//-----------------------------------------------------------------------------
OPfloat OPrenderGetHeightAspectRatio(){
	ASSERT(OPRENDER_HEIGHT > 0, "Height was not greater than 0, there was problem getting width and height");
	aspect = OPRENDER_WIDTH / (OPfloat)OPRENDER_HEIGHT;
	return aspect > 1.0f ? 1.0f : aspect;
}

//-----------------------------------------------------------------------------
void  OPrenderSwapBuffer(){
#ifdef OPIFEX_OPENGL_ES_2
	eglSwapBuffers(display, surface);
#else
	glfwSwapBuffers(window);
#endif
}

void OPrenderUpdate() {
	ASSERT(OPWINDOW_ACTIVE != NULL, "There must be an active window");
#ifdef OPIFEX_OPENGL_ES_2
#else
	glfwPollEvents();
	if (glfwWindowShouldClose(OPWINDOW_ACTIVE->Window)) {
		OPend();
	}
#endif
}

//-----------------------------------------------------------------------------
void  OPrenderPresent(){
#ifdef OPIFEX_OPENGL_ES_2
	eglSwapBuffers(display, surface);
#else
	ASSERT(OPWINDOW_ACTIVE != NULL, "There must be an active window");
	glfwSwapBuffers(OPWINDOW_ACTIVE->Window);
#endif
}
#include "./Human/include/Utilities/Errors.h"
//-----------------------------------------------------------------------------
void  OPrenderDepth(OPint state){
	if(state) {
		glEnable(GL_DEPTH_TEST);
        OPglError("OPrenderDepth:SET Failed");
	} else {
		glDisable(GL_DEPTH_TEST);
	}
}

void OPrenderDepthWrite(OPint state) {
	if (state)
		glDepthMask(GL_TRUE);
	else
		glDepthMask(GL_FALSE);
}



void OPrenderBlend(OPint state) {
	if (state)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);
}
//-----------------------------------------------------------------------------
void  OPrenderShutdown(){
#ifdef OPIFEX_OPENGL_ES_2

#else
	glfwTerminate();
#endif
}

#endif

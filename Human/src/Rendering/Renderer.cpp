#include "./Human/include/Rendering/Renderer.h"

#include "./Core/include/Log.h"
#include "./Core/include/Core.h"
#include "./Core/include/Assert.h"
#include "./Data/include/File.h"


i32 OPrenderWidth;
i32 OPrenderHeight;
i32 OPscreenWidth = 1280;
i32 OPscreenHeight = 720;
f32 OPscreenWidthScale = 1;
f32 OPscreenHeightScale = 1;
OPint OPrenderFullscreen = false;
OPint OPengineHasFocus = 1;
OPint glfwInitialized = 0;

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
			OPlog("INIT FAILED %d", result);
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
			OPlog("INIT FAILED %d", result);
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
void glfwErrorCallback(int error, const char* desc){
	OPlog(desc);
}

void glfwWindowFocusCallback(GLFWwindow* window, int code) {
	OPlog("Focus Result: %d", code);
	OPengineHasFocus = code;
}
#endif

OPint OPrenderInit(){
	OPlog("Initializing Renderer");

#ifdef OPIFEX_ANDROID
	OPscreenWidth = JNIWidth();
	OPscreenHeight = JNIHeight();

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

	OPlog("Getting Display");
	display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	OPlog("Binding API");
	if (eglBindAPI(EGL_OPENGL_ES_API) == GL_FALSE) {
		OPlog("FAILED TO BIND ES API");
	}

	OPlog("eglInitialize");
	eglInitialize(display, 0, 0);


	OPlog("eglMakeCurrent");
	eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

	OPlog("eglChooseConfig");
	/* Here, the application chooses the configuration it desires. In this
	* sample, we have a very simplified selection process, where we pick
	* the first EGLConfig that matches our criteria */
	if (eglChooseConfig(display, attribs, &config, 1, &numConfigs) == EGL_FALSE) {
		OPlog("Choose Config Failed");
	}


	OPlog("eglGetConfigAttrib");
	/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
	* guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
	* As soon as we picked a EGLConfig, we can safely reconfigure the
	* ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);


	OPlog("eglCreateWindowSurface");
	surface = eglCreateWindowSurface(display, config, OPAndroidState->window, NULL);


	OPglError("OPrenderInit:Error 2");

	OPlog("eglCreateContext");
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

	OPscreenWidth = w;
	OPscreenHeight = h;

	OPrenderWidth = OPscreenWidth;
	OPrenderHeight = OPscreenHeight;

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
			OPlog("INIT FAILED %d", result);
			return -1;
		}
	}

	// Most of the below will be moved to a Windowing System
	//glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	GLFWmonitor* monitor = NULL;
	if (OPrenderFullscreen){
		monitor = glfwGetPrimaryMonitor();
	}

	OPint _screenWidth = OPscreenWidth;
	OPint _screenHeight = OPscreenHeight;

	window = glfwCreateWindow(_screenWidth, _screenHeight, "OPifex Entertainment", monitor, NULL);
	OPlog("Created window of size: %d x %d", _screenWidth, _screenHeight);
	glfwGetFramebufferSize(window, &OPscreenWidth, &OPscreenHeight);
	OPscreenWidthScale = _screenWidth / (f32)OPscreenWidth;
	OPscreenHeightScale = _screenHeight / (f32)OPscreenHeight;
	OPlog("Frame Buffer size: %d x %d", OPscreenWidth, OPscreenHeight);
	OPlog("Scale: %f x %f", OPscreenWidthScale, OPscreenHeightScale);

	if(!window) {		
		OPlog("Failed to open GLFW window of %dx%d. If you have an Intel GPU, they are not 3.3 compatible.\n", OPscreenWidth, OPscreenHeight );
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (glfwExtensionSupported("GL_ARB_multisample")) {
		OPlog("Multisampling is supported");
	}

	glfwSetWindowFocusCallback(window, glfwWindowFocusCallback);

	GLint w, h;

	OPrenderSetViewport(0, 0, OPscreenWidth, OPscreenHeight);
	if (glewInit() != GLEW_OK) return -1;	

	glfwSetInputMode(window, GLFW_STICKY_KEYS, true); 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);


	// TODO: Determine how to optimize with this
#if !defined(OPIFEX_OSX32) && !defined(OPIFEX_OSX64)
	//GLuint VertexArrayID;
	//glGenVertexArrays(1, &VertexArrayID);
	//glBindVertexArray(VertexArrayID);
#endif

	glEnable(GL_MULTISAMPLE_ARB);
	glEnable(GL_BLEND); 
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	OPrenderWidth = OPscreenWidth;
	OPrenderHeight = OPscreenHeight;
#endif

	OPlog("OpenGL Context Created W:%d H:%d", OPrenderWidth, OPrenderHeight);
	return 0;
}

void  OPrenderSetScreenSize(OPuint width, OPuint height) {
	OPscreenWidth = width;
	OPscreenHeight = height;
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
void  OPrenderClear(f32 r, f32 g, f32 b){
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//-----------------------------------------------------------------------------
void  OPrenderClearAlpha(f32 r, f32 g, f32 b, f32 a){
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//-----------------------------------------------------------------------------
void  OPrenderSetViewport(OPuint x, OPuint y, OPuint width, OPuint height){
	glViewport(x, y, width, height);
	OPrenderWidth  = width;
	OPrenderHeight = height;
}

void OPrenderResetViewport() {
	OPrenderSetViewport(0, 0, OPscreenWidth, OPscreenHeight);
}

//-----------------------------------------------------------------------------
OPint OPrenderGetWidth(){
#ifndef OPIFEX_ANDROID
	//glfwGetWindowSize(window, &OPrenderWidth, &OPrenderHeight);
#endif
	return OPrenderWidth;
}
//-----------------------------------------------------------------------------
OPint OPrenderGetHeight(){
#ifndef OPIFEX_ANDROID
	//glfwGetWindowSize(window, &OPrenderWidth, &OPrenderHeight);
#endif
	return OPrenderHeight;
}
//-----------------------------------------------------------------------------
OPfloat OPrenderGetAspectRatio(){
	ASSERT(OPrenderGetHeight() > 0, "Height was not greater than 0, there was problem getting width and height");
	return OPrenderGetWidth() / (OPfloat)OPrenderGetHeight();
}

OPfloat aspect;
//-----------------------------------------------------------------------------
OPfloat OPrenderGetWidthAspectRatio(){
	ASSERT(OPrenderWidth > 0, "Height was not greater than 0, there was problem getting width and height");
	aspect = OPrenderHeight / (OPfloat)OPrenderWidth;
	return aspect > 1.0f ? 1.0f : aspect;
}
//-----------------------------------------------------------------------------
OPfloat OPrenderGetHeightAspectRatio(){
	ASSERT(OPrenderHeight > 0, "Height was not greater than 0, there was problem getting width and height");
	aspect = OPrenderWidth / (OPfloat)OPrenderHeight;
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
//-----------------------------------------------------------------------------
void  OPrenderPresent(){
#ifdef OPIFEX_OPENGL_ES_2
	eglSwapBuffers(display, surface);
#else
	glfwSwapBuffers(window);	
	glfwPollEvents();
	if(glfwWindowShouldClose(window)){
		OPend();
	}
#endif
}
//-----------------------------------------------------------------------------
void  OPrenderDepth(OPint state){
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

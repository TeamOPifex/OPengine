#include "./Human/include/Rendering/OPrender.h"

#include "./Core/include/OPlog.h"
#include "./Core/include/OPcore.h"
#include "./Core/include/Assert.h"
#include "./Data/include/OPfile.h"


i32 OPRENDER_WIDTH = 1280;
i32 OPRENDER_HEIGHT = 720;
i32 OPRENDER_SCREEN_WIDTH = 1280;
i32 OPRENDER_SCREEN_HEIGHT = 720;
i32 OPRENDER_SCALED_WIDTH = 1280;
i32 OPRENDER_SCALED_HEIGHT = 720;
f32 OPRENDER_SCREEN_WIDTH_SCALE = 1;
f32 OPRENDER_SCREEN_HEIGHT_SCALE = 1;
OPint OPRENDER_FULLSCREEN = false;
OPint OPRENDER_HAS_FOCUS = 1;
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
#include "./Human/include/Input/OPkeyboard.h"
void glfwCharacterCallback(GLFWwindow* window, unsigned int codepoint)
{
	OPkeyboardKey(codepoint);
}
#endif
OPint OPrenderInit() {
	return OPrenderInit(OPRENDER_SCREEN_WIDTH, OPRENDER_SCREEN_HEIGHT);
}
OPint OPrenderInit(i32 width, i32 height){
	OPRENDER_SCREEN_WIDTH = width;
	OPRENDER_SCREEN_HEIGHT = height;
	OPlogDebug("Initializing Renderer");

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
	//glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);

	GLFWmonitor* monitor = NULL;
	if (OPRENDER_FULLSCREEN){
		monitor = glfwGetPrimaryMonitor();
	}

	OPint _screenWidth = OPRENDER_SCREEN_WIDTH;
	OPint _screenHeight = OPRENDER_SCREEN_HEIGHT;

	OPlog("%d x %d", OPRENDER_SCREEN_WIDTH, OPRENDER_SCREEN_HEIGHT);
	OPlog("%d x %d", _screenWidth, _screenHeight);

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

	if (glfwExtensionSupported("GL_ARB_multisample")) {
		OPlogErr("Multisampling is supported");
	}

	glfwSetWindowFocusCallback(window, glfwWindowFocusCallback);

	GLint w, h;

	glewExperimental = GL_TRUE;
	OPrenderSetViewport(0, 0, OPRENDER_SCREEN_WIDTH, OPRENDER_SCREEN_HEIGHT);
	if (glewInit() != GLEW_OK) return -1;	

	glfwSetInputMode(window, GLFW_STICKY_KEYS, true); 
	
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCharCallback(window, glfwCharacterCallback);
	glfwSetDropCallback(window, glfwWindowDropCallback);

	// TODO: Determine how to optimize with this
//#if !defined(OPIFEX_OSX32) && !defined(OPIFEX_OSX64)
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
//#endif

	glEnable(GL_MULTISAMPLE_ARB);
	glEnable(GL_BLEND); 
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	OPRENDER_WIDTH = OPRENDER_SCREEN_WIDTH;
	OPRENDER_HEIGHT = OPRENDER_SCREEN_HEIGHT;
#endif

	OPlogInfo("OpenGL Context Created W:%d H:%d", OPRENDER_WIDTH, OPRENDER_HEIGHT);
	return 0;
}

void  OPrenderSetScreenSize(OPuint width, OPuint height) {
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
void  OPrenderClear(f32 r, f32 g, f32 b){
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//-----------------------------------------------------------------------------
void  OPrenderClear(f32 r, f32 g, f32 b, f32 a){
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//-----------------------------------------------------------------------------
void  OPrenderClear(OPvec3 color){
	glClearColor(color.x, color.y, color.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
//-----------------------------------------------------------------------------
void  OPrenderClear(OPvec4 color){
	glClearColor(color.x, color.y, color.z, color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//-----------------------------------------------------------------------------
void  OPrenderSetViewport(OPuint x, OPuint y, OPuint width, OPuint height){
	glViewport(x, y, width, height);
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

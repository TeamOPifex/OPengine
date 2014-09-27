#include "./Human/include/Rendering/Renderer.h"

#include "./Core/include/Log.h"
#include "./Core/include/Core.h"
#include "./Core/include/Assert.h"
#include "./Data/include/File.h"


i32 OPrenderWidth;
i32 OPrenderHeight;
i32 OPscreenWidth = 1280;
i32 OPscreenHeight = 720;
OPint OPrenderFullscreen = false;
OPint OPengineHasFocus = 1;
OPint glfwInitialized = 0;

#ifndef OPIFEX_ANDROID
GLFWwindow* window = NULL;
#endif

ui32 OPgetNativeScreenWidth() {
	if (!glfwInitialized) {
		int result = glfwInit();
		if (!result) {
			OPlog("INIT FAILED %d", result);
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
			OPlog("INIT FAILED %d", result);
			return -1;
		}
		glfwInitialized = 1;
	}
	return glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
}

void glfwErrorCallback(int error, const char* desc){
	OPlog(desc);
}

void glfwWindowFocusCallback(GLFWwindow* window, int code) {
	OPlog("Focus Result: %d", code);
	OPengineHasFocus = code;
}

OPint OPrenderInit(){

#ifdef OPIFEX_ANDROID
	OPscreenWidth = JNIWidth();
	OPscreenHeight = JNIHeight();
#endif

#ifdef OPIFEX_OPENGL_ES_2
	// Android doesn't need to create a window
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 
	//glCullFace(GL_FRONT);
	//glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glEnable( GL_BLEND );

	OPrenderWidth = width;
	OPrenderHeight = height;
	OPscreenWidth = width;
	OPscreenHeight = height;
#else	

	OPstream* str = OPreadFile("../app.config");
	if (str) {
		OPrenderFullscreen = OPstreamI32(str);
		OPscreenWidth = OPstreamI32(str);
		if (OPscreenWidth == -1) OPscreenWidth = OPgetNativeScreenWidth();
		OPscreenHeight = OPstreamI32(str);
		if (OPscreenHeight == -1) OPscreenHeight = OPgetNativeScreenHeight();
	}

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

	window = glfwCreateWindow(OPscreenWidth, OPscreenHeight, "OPifex Entertainment", monitor, NULL);
	glfwGetFramebufferSize(window, &OPscreenWidth, &OPscreenHeight);

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
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
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
	glfwGetWindowSize(window, &OPrenderWidth, &OPrenderHeight);
#endif
	return OPrenderWidth;
}
//-----------------------------------------------------------------------------
OPint OPrenderGetHeight(){
#ifndef OPIFEX_ANDROID
	glfwGetWindowSize(window, &OPrenderWidth, &OPrenderHeight);
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
	
#else
	glfwSwapBuffers(window);	
#endif
}
//-----------------------------------------------------------------------------
void  OPrenderPresent(){
#ifdef OPIFEX_OPENGL_ES_2
	
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

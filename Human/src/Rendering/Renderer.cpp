#include "./Core/include/Log.h"
#include "./Human/include/Rendering/OPrenderer.h"
#include "./Core/include/Assert.h"

i32 OPrenderWidth;
i32 OPrenderHeight;
GLFWwindow* window = NULL;

void glfwErrorCallback(int error, const char* desc){
	OPLog(desc);
}

OPint OPrenderInit(ui32 width, ui32 height, bool fullscreen){


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

	return 0;
#else
	GLuint VertexArrayID;
	
	glfwSetErrorCallback(glfwErrorCallback);

	int result = glfwInit();
	if( !result ) {
		OPLog("INIT FAILED %d", result);
		return -1;
	} else {
		OPLog("INIT SUCCEEDED %d", result);
	}


	
	// Most of the below will be moved to a Windowing System
	
	//glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	#if defined(OPIFEX_OSX32) || defined(OPIFEX_OSX64)
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "OPifex Entertainment", NULL, NULL);
	if(!window) {		
		OPLog("Failed to open GLFW window of %dx%d. If you have an Intel GPU, they are not 3.3 compatible.\n", width, height );
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(NULL);
	glfwMakeContextCurrent(window);

	GLFWwindow* tmp = glfwGetCurrentContext();
	if(!tmp || tmp != window) {
		OPLog("FAILED to bind context");
	} else {
		OPLog("SUCCEEDED to bind context");
	}

	if( !glfwInit() ) {
		OPLog("INIT FAILED");
		return -1;
	} else {
		OPLog("INIT SUCCEEDED");
	}

	// Open a window and create its OpenGL context
	// if( !glfwOpenWindow( width,	 height, 0,0,0,0, 32,0, GLFW_WINDOW ) )
	// {		
	// }

	OPrenderSetViewport(0, 0, width, height);
	if (glewInit() != GLEW_OK) return -1;	

	if (glewGetExtension("GL_ARB_fragment_program"))
	{
	  /* Looks like ARB_fragment_program is supported. */
		OPLog("YES: GL_ARB_fragment_program");
	} else {
		OPLog("NO: GL_ARB_fragment_program");
	}

	//glfwSetWindowTitle( "OPifex Engine" );
	glfwSetInputMode(window, GLFW_STICKY_KEYS, true);
	//glfwSetInputMode(window, GL_DEPTH_TEST, true);
	//glfwSetInputMode(window, GL_CULL_FACE, false);
	
	// TODO: Determine how to optimize with this
	//glGenVertexArrays(1, &VertexArrayID);
	//glBindVertexArray(VertexArrayID);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	OPrenderWidth = width;
	OPrenderHeight = height;

	OPLog("INIT w:%d h:%d\n", OPrenderWidth, OPrenderHeight);	

	return 0;
#endif
}
//-----------------------------------------------------------------------------
void  OPrenderClear(f32 r, f32 g, f32 b){
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
//-----------------------------------------------------------------------------
void  OPrenderSetViewport(ui32 x, ui32 y, ui32 width, ui32 height){
	glViewport(x, y, width, height);
	OPrenderWidth  = width;
	OPrenderHeight = height;
}
//-----------------------------------------------------------------------------
OPint OPrenderGetWidth(){
	glfwGetWindowSize(window, &OPrenderWidth, &OPrenderHeight);
	return OPrenderWidth;
}
//-----------------------------------------------------------------------------
OPint OPrenderGetHeight(){
	glfwGetWindowSize(window, &OPrenderWidth, &OPrenderHeight);
	return OPrenderHeight;
}
//-----------------------------------------------------------------------------
OPfloat OPrenderGetAspectRatio(){
	glfwGetWindowSize(window, &OPrenderWidth, &OPrenderHeight);
	ASSERT(OPrenderWidth > 0, "Height was not greater than 0, there was problem getting width and height");
	return OPrenderHeight / (OPfloat)OPrenderWidth;
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
#endif
}
//-----------------------------------------------------------------------------
void  OPrenderDepth(OPint state){
	if(state)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}
//-----------------------------------------------------------------------------
void  OPrenderShutdown(){
#ifdef OPIFEX_OPENGL_ES_2

#else
	glfwTerminate();
#endif
}

#include "./Core/include/Log.h"
#include "./Human/include/Rendering/OPrenderer.h"
#include "./Core/include/Assert.h"

OPint OPrenderWidth;
OPint OPrenderHeight;

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
	
	// Most of the below will be moved to a Windowing System
	if( !glfwInit() ) return -1;
	
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	if( !glfwOpenWindow( width, height, 8,8,8,8, 32,0, fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOW ) )
	{		
		OPLog("Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.\n" );
		glfwTerminate();
		return -1;
	}

	GLFWvidmode mode;
	glfwGetDesktopMode(&mode);
	glfwSetWindowPos((mode.Width - width) / 2, (mode.Height - height) / 2);

	OPrenderSetViewport(0, 0, width, height);
	if (glewInit() != GLEW_OK) return -1;	

	glfwSetWindowTitle( "OPifex Engine" );
	glfwEnable( GLFW_STICKY_KEYS );	

	glEnable(GL_DEPTH_TEST);

	glDisable(GL_CULL_FACE);
	
	// TODO: Determine how to optimize with this
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	
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
	glfwGetWindowSize(&OPrenderWidth, &OPrenderHeight);
	return OPrenderWidth;
}
//-----------------------------------------------------------------------------
OPint OPrenderGetHeight(){
	glfwGetWindowSize(&OPrenderWidth, &OPrenderHeight);
	return OPrenderHeight;
}
//-----------------------------------------------------------------------------
OPfloat OPrenderGetAspectRatio(){
	glfwGetWindowSize(&OPrenderWidth, &OPrenderHeight);
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
	glfwSwapBuffers();	
#endif
}
//-----------------------------------------------------------------------------
void  OPrenderPresent(){
#ifdef OPIFEX_OPENGL_ES_2
	
#else
	glfwSwapBuffers();	
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

#include "./Core/include/Log.h"
#include "./Human/include/Rendering/Renderer.h"

OPint OPrenderWidth, OPrenderHeight;

OPint OPrenderInit(ui32 width, ui32 height){
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
	if( !glfwOpenWindow( width, height, 0,0,0,0, 32,0, GLFW_WINDOW ) )
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

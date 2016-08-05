#include "./Human/include/Rendering/API/OPrenderer.h"
#include "./Human/include/Platform/opengl/OPcommonGL.h"
#include "./Human/include/Platform/opengl/OPwindowAPIGL.h"
#include "./Human/include/Rendering/OPmonitor.h"
#include "./Human/include/Rendering/OPglew.h"
#include "./Human/include/Rendering/OPwindow.h"
#include "./Core/include/Assert.h"

// TODO: (garrett) CMake should be generating these variables and we'll check for just OPIFEX_OPENGL and use the OPIFEX_OPENGL_MAJOR and OPIFEX_OPENGL_MINOR
#if defined(OPIFEX_OPENGL_3_3)
#define OPIFEX_OPENGL 1
#define OPIFEX_OPENGL_MAJOR 3
#define OPIFEX_OPENGL_MINOR 3
#endif

void glfwErrorCallback(int error, const char* desc) {
	OPlogErr("GLFW ERROR: %s", desc);
}

i8 OPrendererInitGL(OPwindow* window) {
	OPlogInfo("Initializing OpenGL Renderer");

	glfwSetErrorCallback(glfwErrorCallback);
	
	window->Bind();

	glEnable(GL_MULTISAMPLE_ARB);
	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);

	if (OPglewInit() != 0) {
		OPlogErr("Failed to initialize GLEW");
		return -2;
	}

	OPRENDERER_ACTIVE->SetBlendModeAlpha();

	return 0;
}

void OPrendererClearGL(OPvec4 color){
	OPGLFN(glClearColor(color.x, color.y, color.z, color.w));
	OPGLFN(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void OPrendererPresentGL() {
	ASSERT(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE != NULL, "There must be an active window");
	OPwindowGL* windowGL = (OPwindowGL*)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->internalPtr;
	OPGLFN(glfwSwapBuffers(windowGL->Handle));
}

void OPrendererSetDepthTestingGL(bool state) {
	if (state) {
		OPGLFN(glEnable(GL_DEPTH_TEST));
	}
	else {
		OPGLFN(glDisable(GL_DEPTH_TEST));
	}
}

void OPrendererSetDepthWriteGL(bool state){
	OPGLFN(glDepthMask(state ? GL_TRUE : GL_FALSE));
}

void OPrendererSetCullGL(bool state) {
	if (state) {
		OPGLFN(glEnable(GL_CULL_FACE));
	}
	else {
		OPGLFN(glDisable(GL_CULL_FACE));
	}
}

void OPrendererSetCullModeGL(i8 state) {
	if (state) {
		OPGLFN(glCullFace(GL_FRONT));
	}
	else {
		OPGLFN(glCullFace(GL_BACK));
	}
}

void OPrendererSetBlendGL(bool state){
	if (state) {
		OPGLFN(glEnable(GL_BLEND));
	}
	else {
		OPGLFN(glDisable(GL_BLEND));
	}
}


ui32 OPblendFunctionToGL(OPblendFunction blendFunction) {
	switch (blendFunction)
	{
		case OPblendFunction::CONSTANT_ALPHA: return GL_CONSTANT_ALPHA;
		case OPblendFunction::CONSTANT_COLOR: return GL_CONSTANT_COLOR;
		case OPblendFunction::DST_ALPHA: return GL_DST_ALPHA;
		case OPblendFunction::DST_COLOR: return GL_DST_COLOR;
		case OPblendFunction::ONE: return GL_ONE;
		case OPblendFunction::ONE_MINUS_CONSTANT_ALPHA: return GL_ONE_MINUS_CONSTANT_ALPHA;
		case OPblendFunction::ONE_MINUS_CONSTANT_COLOR: return GL_ONE_MINUS_CONSTANT_COLOR;
		case OPblendFunction::ONE_MINUS_DST_ALPHA: return GL_ONE_MINUS_DST_ALPHA;
		case OPblendFunction::ONE_MINUS_DST_COLOR: return GL_ONE_MINUS_DST_COLOR;
		case OPblendFunction::ONE_MINUS_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA;
		case OPblendFunction::ONE_MINUS_SRC_COLOR: return GL_ONE_MINUS_SRC_COLOR;
		case OPblendFunction::SRC_ALPHA: return GL_SRC_ALPHA;
		case OPblendFunction::SRC_ALPHA_SATURATE: return GL_SRC_ALPHA_SATURATE;
		case OPblendFunction::SRC_COLOR: return GL_SRC_COLOR;
		case OPblendFunction::ZERO: return GL_ZERO;
	}
	return 0;
}

void OPrendererSetBlendModeGL(OPblendFunction src, OPblendFunction dst) {
	OPGLFN(glBlendFunc(OPblendFunctionToGL(src), OPblendFunctionToGL(dst)));	
}

inline void OPrenderSetViewportGL(OPuint x, OPuint y, OPuint width, OPuint height) {
	OPGLFN(glViewport(x, y, width, height));
}

void OPrendererSwapBufferGL() {
	ASSERT(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE != NULL, "There must be an active window");
	OPwindowGL* windowGL = (OPwindowGL*)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->internalPtr;
	OPGLFN(glfwSwapBuffers(windowGL->Handle));
}

void OPrendererShutdownGL() {
	glfwTerminate();
}


#include "./Human/include/Platform/opengl/OPcontextGL.h"
#include "./Human/include/Platform/opengl/OPeffectAPIGL.h"
#include "./Human/include/Platform/opengl/OPframeBuffer2DGL.h"
#include "./Human/include/Platform/opengl/OPframeBufferDepthGL.h"
#include "./Human/include/Platform/opengl/OPindexBufferGL.h"
#include "./Human/include/Platform/opengl/OPmonitorAPIGL.h"
#include "./Human/include/Platform/opengl/OPshaderAPIGL.h"
#include "./Human/include/Platform/opengl/OPshaderAttributeAPIGL.h"
#include "./Human/include/Platform/opengl/OPshaderResourceGL.h"
#include "./Human/include/Platform/opengl/OPshaderUniformAPIGL.h"
#include "./Human/include/Platform/opengl/OPtextureAPIGL.h"
#include "./Human/include/Platform/opengl/OPtexture2DGL.h"
#include "./Human/include/Platform/opengl/OPtextureCubeGL.h"
#include "./Human/include/Platform/opengl/OPtextureDepthGL.h"
#include "./Human/include/Platform/opengl/OPvertexArrayAPIGL.h"
#include "./Human/include/Platform/opengl/OPvertexBufferAPIGL.h"
#include "./Human/include/Platform/opengl/OPwindowAPIGL.h"

OPrenderer OPRENDERERGL;
OPrenderer* OPrendererGL() {
	OPRENDERERGL.Init = OPrendererInitGL;
	OPRENDERERGL._Clear = OPrendererClearGL;
	OPRENDERERGL.Present = OPrendererPresentGL;
	OPRENDERERGL.SetDepthTesting = OPrendererSetDepthTestingGL;
	OPRENDERERGL.SetDepthWrite = OPrendererSetDepthWriteGL;
	OPRENDERERGL.SetCull = OPrendererSetCullGL;
	OPRENDERERGL.SetCullMode = OPrendererSetCullModeGL;
	OPRENDERERGL.SetBlend = OPrendererSetBlendGL;
	OPRENDERERGL.SetBlendMode = OPrendererSetBlendModeGL;
	OPRENDERERGL.SetViewport = OPrenderSetViewportGL;
	OPRENDERERGL.SwapBuffer = OPrendererSwapBufferGL;
	OPRENDERERGL.Shutdown = OPrendererShutdownGL;

	OPcontextGLInit(&OPRENDERERGL.Context); 
	OPeffectAPIGLInit(&OPRENDERERGL.Effect);
	OPframeBuffer2DGLInit(&OPRENDERERGL.FrameBuffer2D);
	OPframeBufferDepthGLInit(&OPRENDERERGL.FrameBufferDepth);
	OPindexBufferAPIGLInit(&OPRENDERERGL.IndexBuffer);
	OPmonitorAPIGLInit(&OPRENDERERGL.Monitor);
	OPshaderAPIGLInit(&OPRENDERERGL.Shader);
	OPshaderAttributeAPIGLInit(&OPRENDERERGL.ShaderAttribute);
	OPshaderResourceGLInit(&OPRENDERERGL.ShaderResource);
	OPshaderUniformAPIGLInit(&OPRENDERERGL.ShaderUniform);
	OPtextureAPIGLInit(&OPRENDERERGL.Texture);
	OPtexture2DGLInit(&OPRENDERERGL.Texture2D);
	OPtextureCubeGLInit(&OPRENDERERGL.TextureCube);
	OPtextureDepthGLInit(&OPRENDERERGL.TextureDepth);
	OPvertexArrayAPIGLInit(&OPRENDERERGL.VertexArray);
	OPvertexBufferAPIGLInit(&OPRENDERERGL.VertexBuffer);
	OPwindowAPIGLInit(&OPRENDERERGL.Window);

	return &OPRENDERERGL;
}
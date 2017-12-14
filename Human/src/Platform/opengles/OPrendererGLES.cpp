#include "./Human/include/Rendering/API/OPrenderer.h"

#ifdef OPIFEX_OPENGL_ES_2

#include "./Human/include/Platform/opengles/OPcommonGLES.h"
#include "./Human/include/Platform/opengles/OPwindowAPIGLES.h"
#include "./Human/include/Rendering/Enums/OPcullFace.h"
#include "./Human/include/Rendering/OPmonitor.h"
#include "./Human/include/Rendering/OPwindow.h"
#include "./Core/include/Assert.h"

#define OPIFEX_OPENGL_MAJOR 2
#define OPIFEX_OPENGL_MINOR 0

#include <GLES2/gl2.h>


i8 OPrendererInitGLES(OPwindow* window) {
	OPlogInfo("Initializing OpenGL Renderer");
	
	OPRENDERER_ACTIVE->SetBlendModeAlpha();

	return 0;
}

void OPrendererClearGLES(OPvec4 color) {
	OPGLFN(glClearColor(color.x, color.y, color.z, color.w));
	OPGLFN(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void OPrendererClearColorGLES(OPvec4 color) {
	OPGLFN(glClearColor(color.x, color.y, color.z, color.w));
	OPGLFN(glClear(GL_COLOR_BUFFER_BIT));
}

void OPrendererClearDepthGLES() {
	OPGLFN(glClear(GL_DEPTH_BUFFER_BIT));
}

void OPrendererPresentGLES() {
	ASSERT(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE != NULL, "There must be an active window");
	OPwindowGLES* windowGL = (OPwindowGLES*)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->internalPtr;

	// OPGLFN(glfwSwapBuffers(windowGL->Handle));
}

void OPrendererSetDepthTestingGLES(bool state) {
	if (state) {
		OPGLFN(glEnable(GL_DEPTH_TEST));
	}
	else {
		OPGLFN(glDisable(GL_DEPTH_TEST));
	}
}

void OPrendererSetDepthWriteGLES(bool state){
	if (state) {
		OPGLFN(glDepthMask(GL_TRUE));
	}
	else {
		OPGLFN(glDepthMask(GL_FALSE));
	}
}

void OPrendererSetCullGLES(bool state) {
	if (state) {
		OPGLFN(glEnable(GL_CULL_FACE));
	}
	else {
		OPGLFN(glDisable(GL_CULL_FACE));
	}
}

void OPrendererSetMultisampleGLES(bool state) {
	// if (state) {
	// 	OPGLFN(glEnable(GL_MULTISAMPLE));
	// }
	// else {
	// 	OPGLFN(glDisable(GL_MULTISAMPLE));
	// }
}

void OPrendererSetWireframeGLES(bool state) {
	// if (state) {
	// 	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// }
	// else {
	// 	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// }
}

void OPrendererSetCullModeGLES(OPcullFace state) {
	OPGLFN(glCullFace(state == OPcullFace::FRONT ? GL_FRONT : GL_BACK));
}

void OPrendererSetBlendGLES(bool state){
	if (state) {
		OPGLFN(glEnable(GL_BLEND));
	}
	else {
		OPGLFN(glDisable(GL_BLEND));
	}
}


ui32 OPblendFunctionToGLES(OPblendFunction::Enum blendFunction) {
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

void OPrendererSetBlendModeGLES(OPblendFunction::Enum src, OPblendFunction::Enum dst) {
	OPGLFN(glBlendFunc(OPblendFunctionToGLES(src), OPblendFunctionToGLES(dst)));
}

inline void OPrenderSetViewportGLES(ui32 x, ui32 y, ui32 width, ui32 height) {
   // OPlogInfo("Set viewport %dx%d", width, height);
	OPGLFN(glViewport(x, y, width, height));
}

void OPrendererSwapBufferGLES() {
	ASSERT(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE != NULL, "There must be an active window");
	OPwindowGL* windowGL = (OPwindowGL*)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->internalPtr;
	//OPGLFN(glfwSwapBuffers(windowGL->Handle));
}

void OPrendererShutdownGLES() {
	
}


#include "./Human/include/Platform/opengles/OPcontextGLES.h"
#include "./Human/include/Platform/opengles/OPeffectAPIGLES.h"
#include "./Human/include/Platform/opengles/OPframeBufferAPIGLES.h"
#include "./Human/include/Platform/opengles/OPframeBufferDepthAPIGLES.h"
#include "./Human/include/Platform/opengles/OPindexBufferGLES.h"
#include "./Human/include/Platform/opengles/OPmonitorAPIGLES.h"
#include "./Human/include/Platform/opengles/OPshaderAPIGLES.h"
#include "./Human/include/Platform/opengles/OPshaderAttributeAPIGLES.h"
#include "./Human/include/Platform/opengles/OPshaderResourceGLES.h"
#include "./Human/include/Platform/opengles/OPshaderUniformAPIGLES.h"
#include "./Human/include/Platform/opengles/OPtextureAPIGLES.h"
#include "./Human/include/Platform/opengles/OPtextureCubeAPIGLES.h"
#include "./Human/include/Platform/opengles/OPvertexArrayAPIGLES.h"
#include "./Human/include/Platform/opengles/OPvertexBufferAPIGLES.h"
#include "./Human/include/Platform/opengles/OPwindowAPIGLES.h"

OPrenderAPI OPRENDERERGLES;
OPrenderAPI* OPrendererGLES() {
	OPRENDERERGLES.Init = OPrendererInitGLES;
	OPRENDERERGLES._Clear = OPrendererClearGLES;
	OPRENDERERGLES._ClearColor = OPrendererClearColorGLES;
	OPRENDERERGLES.ClearDepth = OPrendererClearDepthGLES;
	OPRENDERERGLES.Present = OPrendererPresentGLES;
	OPRENDERERGLES.SetDepthTesting = OPrendererSetDepthTestingGLES;
	OPRENDERERGLES.SetDepthWrite = OPrendererSetDepthWriteGLES;
	OPRENDERERGLES.SetCull = OPrendererSetCullGLES;
	OPRENDERERGLES.SetCullMode = OPrendererSetCullModeGLES;
	OPRENDERERGLES.SetMultisample = OPrendererSetMultisampleGLES;
	OPRENDERERGLES.SetWireframe = OPrendererSetWireframeGLES;
	OPRENDERERGLES.SetBlend = OPrendererSetBlendGLES;
	OPRENDERERGLES.SetBlendMode = OPrendererSetBlendModeGLES;
	OPRENDERERGLES.SetViewport = OPrenderSetViewportGLES;
	OPRENDERERGLES.SwapBuffer = OPrendererSwapBufferGLES;
	OPRENDERERGLES.Shutdown = OPrendererShutdownGLES;

	OPcontextGLESInit(&OPRENDERERGLES.Context);
	OPeffectAPIGLESInit(&OPRENDERERGLES.Effect);
	OPframeBufferAPIGLESInit(&OPRENDERERGLES.FrameBuffer);
	OPframeBufferDepthAPIGLESInit(&OPRENDERERGLES.FrameBufferDepth);
	OPindexBufferAPIGLESInit(&OPRENDERERGLES.IndexBuffer);
	OPmonitorAPIGLESInit(&OPRENDERERGLES.Monitor);
	OPshaderAPIGLESInit(&OPRENDERERGLES.Shader);
	OPshaderAttributeAPIGLESInit(&OPRENDERERGLES.ShaderAttribute);
	OPshaderResourceGLESInit(&OPRENDERERGLES.ShaderResource);
	OPshaderUniformAPIGLESInit(&OPRENDERERGLES.ShaderUniform);
	OPtextureAPIGLESInit(&OPRENDERERGLES.Texture);
	OPtextureCubeAPIGLESInit(&OPRENDERERGLES.TextureCube);
	OPvertexArrayAPIGLESInit(&OPRENDERERGLES.VertexArray);
	OPvertexBufferAPIGLESInit(&OPRENDERERGLES.VertexBuffer);
	OPwindowAPIGLESInit(&OPRENDERERGLES.Window);

	OPlogErr("Renderer ES 2 Setup");

	return &OPRENDERERGLES;
}

#endif
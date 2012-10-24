#define OPIFEX_ANDROID
#include "Human/Rendering/RenderSystem.h"
//#include "Human\Rendering\GL\ES\GLESShader.h"
//#include "Human\Rendering\GL\ES\GLESMaterial.h"
#include <jni.h>

//static const char gVertexShader[] = 
//    "attribute vec4 vPosition;\n"
//    "void main() {\n"
//    "  gl_Position = vPosition;\n"
//    "}\n";
//
//static const char gFragmentShader[] = 
//    "precision mediump float;\n"
//    "void main() {\n"
//    "  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
//    "}\n";
//	
//	static const f32 g_vertex_buffer_data[] = { 
//		-1.0f, -1.0f, 0.0f,
//		 1.0f, -1.0f, 0.0f,
//		 0.0f,  1.0f, 0.0f,
//	};
//	
//f32 gTriangleVertices[] = { 0.0f, 0.5f, -0.5f, -0.5f,
//        0.5f, -0.5f };

extern "C" {
    JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_step(JNIEnv * env, jobject obj);
};

//MaterialPtr material;

JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
	RenderSystem::Initialize(OpenGL_ES_2_0);
	
	//ShaderPtr vertex = new GLESShader(Vertex, gVertexShader);
	//ShaderPtr pixel = new GLESShader(Fragment, gFragmentShader);
	
	//material = new GLESMaterial(vertex, pixel);	
}

JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_step(JNIEnv * env, jobject obj)
{
	RenderSystem::ClearColor(0.0f, 0.0f, 1.0f);
	//RenderSystem::UseMaterial(material);
	//material->set_data(0, 2, false, 0, gTriangleVertices);
	//material->enable_attrib(0);
	//RenderSystem::RenderTriangles(0, 3);
	//material->disable_attrib(0);
	RenderSystem::Present();
}

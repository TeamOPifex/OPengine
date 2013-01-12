#define OPIFEX_ANDROID

#include "Human/Rendering/RenderSystem.h"
#include "Human/Resources/Texture/TextureDDS.h"
#include "Human\Rendering\GL\GLTexture.h"
#include "Human\Rendering\GL\GLShader.h"
#include "Human\Rendering\GL\GLMaterial.h"
#include "Human\Rendering\GL\GLBuffer.h"
#include "Human\Math\Matrix4.h"
#include <jni.h>

#include <sys/types.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "Core\include\Log.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <android/log.h>

#include "Data\include\OPfile.h"

static const char gVertexShader[] = 
    "attribute vec3 vPosition; \n"
    "attribute vec2 TexCoordIn; \n"
    "uniform mat4 MVP; \n"
    "varying vec2 TexCoordOut; \n"
    "void main() {\n"
	"  gl_Position = MVP * vec4(vPosition,1); \n"
    "  TexCoordOut = TexCoordIn; \n"
    "}\n";

static const char gFragmentShader[] = 
	"varying lowp vec2 TexCoordOut; \n"
	"uniform sampler2D Texture; \n"
    "void main() {\n"
	"  gl_FragColor = texture2D(Texture, TexCoordOut);\n"
    "}\n";

static const f32 g_uv_buffer_data[] = { 
		0.000059f, 1.0f-0.000004f, 
		0.000103f, 1.0f-0.336048f, 
		0.335973f, 1.0f-0.335903f, 
		1.000023f, 1.0f-0.000013f, 
		0.667979f, 1.0f-0.335851f, 
		0.999958f, 1.0f-0.336064f, 
		0.667979f, 1.0f-0.335851f, 
		0.336024f, 1.0f-0.671877f, 
		0.667969f, 1.0f-0.671889f, 
		1.000023f, 1.0f-0.000013f, 
		0.668104f, 1.0f-0.000013f, 
		0.667979f, 1.0f-0.335851f, 
		0.000059f, 1.0f-0.000004f, 
		0.335973f, 1.0f-0.335903f, 
		0.336098f, 1.0f-0.000071f, 
		0.667979f, 1.0f-0.335851f, 
		0.335973f, 1.0f-0.335903f, 
		0.336024f, 1.0f-0.671877f, 
		1.000004f, 1.0f-0.671847f, 
		0.999958f, 1.0f-0.336064f, 
		0.667979f, 1.0f-0.335851f, 
		0.668104f, 1.0f-0.000013f, 
		0.335973f, 1.0f-0.335903f, 
		0.667979f, 1.0f-0.335851f, 
		0.335973f, 1.0f-0.335903f, 
		0.668104f, 1.0f-0.000013f, 
		0.336098f, 1.0f-0.000071f, 
		0.000103f, 1.0f-0.336048f, 
		0.000004f, 1.0f-0.671870f, 
		0.336024f, 1.0f-0.671877f, 
		0.000103f, 1.0f-0.336048f, 
		0.336024f, 1.0f-0.671877f, 
		0.335973f, 1.0f-0.335903f, 
		0.667969f, 1.0f-0.671889f, 
		1.000004f, 1.0f-0.671847f, 
		0.667979f, 1.0f-0.335851f
	};

f32 gTriangleVertices[] = { 
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f
	};

MaterialPtr material;
GLTexture* tex;
ui32 sampLoc;
BufferPtr buffer;
BufferPtr uv;
ui32 mvpLoc;
Matrix4 m;
ui32 bufferLoc;
ui32 uvLoc;
Matrix4 rotate;
Matrix4 v, p;

extern "C" {
    JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height, jobject assetManager);
    JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_step(JNIEnv * env, jobject obj);
};

JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height, jobject assetManager)
{
	//__android_log_print(ANDROID_LOG_ERROR, "OPIFEX", (const char*)glGetString(GL_EXTENSIONS));
	
	AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);
	if(mgr == NULL)
		OPLog("Asset manager not created.");
	OPfileInit(mgr);

	RenderSystem::Initialize();

	m.SetIdentity();
	p = Matrix4::CreatePerspective(45.0f, 1280.0f / 800.0f, 1.0f, 100.0f);
	v = Matrix4::CreateLook(Vector3(4,3,3), Vector3(0), Vector3(0,1,0));

	rotate = Matrix4::RotateY(0.025f);

	//rotate.SetIdentity();
	
	//rotate[1][1] = 0.99875026039f;
	//rotate[2][1] = -0.04997916927f;
	//rotate[1][2] = 0.04997916927f;
	//rotate[2][2] = 0.99875026039f;

	ShaderPtr vertex = new GLShader(Vertex, gVertexShader);
	ShaderPtr pixel = new GLShader(Fragment, gFragmentShader);

	material = new GLMaterial(vertex, pixel);	
	mvpLoc = material->uniform_location("MVP");
	sampLoc = material->uniform_location("Texture");

	bufferLoc = material->attribute_location("vPosition");
	uvLoc = material->attribute_location("TexCoordIn");


	// This is how it should work eventually
	//OPstream* file = OPreadFile("bricks.dds.mp3");


	//####
	//#### Below should be removed in favor of OPreadFile
	//####
	AAsset* asset = AAssetManager_open(mgr, "bricks.dds.mp3", AASSET_MODE_UNKNOWN);
	if(asset == NULL)
		OPLog("Asset not loaded.");

	off_t start, length;
    int fd = AAsset_openFileDescriptor(asset, &start, &length);

    FILE* myFile = fdopen(dup(fd), "rb"); 
	if(!myFile){
		OPLog("File not loaded.");
		return;
	}
	fseek(myFile, start, SEEK_SET);
	//####
	//#### Above should be removed in favor of OPreadFile
	//####


	// TextureDDS Should take a stream
	Texture* dds = new TextureDDS(myFile);	
	tex = new GLTexture(dds);
	delete(dds);

	buffer = new GLBuffer(0, sizeof(f32) * 108, gTriangleVertices);
	uv = new GLBuffer(0, sizeof(f32) * 72, g_uv_buffer_data);
	
	OPLog("Initialized Successfully");
}
bool firstRun = false;
Matrix4 result;

JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_step(JNIEnv * env, jobject obj){
	RenderSystem::ClearColor(0.0f, 0.0f, 1.0f);

	// Set material to use
	RenderSystem::UseMaterial(material);
	
	// Set MVP Matrix
	m = m * rotate;
	result = m * v * p;
	material->set_matrix(mvpLoc, &result[0][0]);
	
	// Set Texture
	material->enable_attrib(sampLoc);
	tex->bind(sampLoc);
	
	// Set Vertex Buffer
	material->enable_attrib(bufferLoc);
	RenderSystem::SetBuffer(buffer->handle());
	material->set_data(bufferLoc, 3, false, 0, (void*)0);
			
	// Set UV Buffer
	material->enable_attrib(uvLoc);
	RenderSystem::SetBuffer(uv->handle());
	material->set_data(uvLoc, 2, false, 0, (void*)0);

	if(!firstRun){
		OPLog_i32(buffer->handle());
		OPLog_i32(uv->handle());
	}

	// Render the scene
	RenderSystem::RenderTriangles(0, 12*3);	
	RenderSystem::Present();

	// Clean up
	material->disable_attrib(bufferLoc);
	material->disable_attrib(uvLoc);
	material->disable_attrib(sampLoc);

	firstRun = true;
}

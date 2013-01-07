#define OPIFEX_ANDROID

#include "Human/Rendering/RenderSystem.h"
#include "Human/Resources/Texture/Texture.h"
#include "Human\Rendering\GL\ES\GLESShader.h"
#include "Human\Rendering\GL\ES\GLESMaterial.h"
#include "Human\Rendering\GL\ES\GLESBuffer.h"
#include "Human\Math\Matrix4.h"
#include <jni.h>

#include <sys/types.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "Core\include\Log.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


static const char gVertexShader[] = 
	"#version 330 core\n"
    "layout(location = 0) in vec3 vertexPosition_modelspace; \n"
	"layout(location = 1) in vec2 vertexUV;"
	"out vec2 UV;\n"
	"uniform mat4 MVP;\n"
	"void main(){\n"
    "gl_Position = MVP * vec4(vertexPosition_modelspace,1);\n"
	"UV = vertexUV;\n"
	"}";

static const char gFragmentShader[] = 
	"#version 330 core \n"
	"in vec2 UV; \n"
	"out vec3 color; \n"
	"uniform sampler2D myTextureSampler; \n"
    "void main() { \n"
	"  color = texture2D( myTextureSampler, UV ).rgb; \n"
    "} \n";

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

extern "C" {
    JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height, jobject assetManager);
    JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_step(JNIEnv * env, jobject obj);
};

JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height, jobject assetManager)
{
	RenderSystem::Initialize(OpenGL_ES_2_0);

	Matrix4 v, p;
	m.SetIdentity();
	p = Matrix4::CreatePerspective(45.0f, 4.0f / 3.0f, 1.0f, 100.0f);
	v = Matrix4::CreateLook(Vector3(4,3,3), Vector3(0), Vector3(0,1,0));
	m = m * v * p;

	ShaderPtr vertex = new GLESShader(Vertex, gVertexShader);
	ShaderPtr pixel = new GLESShader(Fragment, gFragmentShader);

	material = new GLESMaterial(vertex, pixel);	
	mvpLoc = material->uniform_location("MVP");
	sampLoc = material->uniform_location("myTextureSampler");

	AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);
	if(mgr == NULL)
		OPLog("Asset manager not created.");

	AAsset* asset = AAssetManager_open(mgr, "bricks.dds.mp3", AASSET_MODE_UNKNOWN);
	if(asset == NULL)
		OPLog("Asset not loaded.");

	off_t start, length;
    int fd = AAsset_openFileDescriptor(asset, &start, &length);
    FILE* myFile = fdopen(dup(fd), "rb"); 
	if(!myFile){
		OPLog("File not loaded.");
	}
	fseek(myFile, start, SEEK_SET);
	TextureDDS* dds = new TextureDDS(myFile);

	if(!dds){
		OPLog("Texture not loaded.");
	}
	
	tex = new GLTexture(dds);
	delete(dds);

	buffer = new GLESBuffer(0, sizeof(gTriangleVertices), gTriangleVertices);

	uv = new GLESBuffer(0, sizeof(g_uv_buffer_data), g_uv_buffer_data);

	OPLog("Vertices");
	for(int i = 0; i < 20; i++){
		OPLogFloat(gTriangleVertices[i]);
	}	

	OPLog("UVs");
	for(int i = 0; i < 20; i++){
		OPLogFloat(g_uv_buffer_data[i]);
	}

	OPLog("Initialized Successfully");
}

bool firstRun = true;

JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_step(JNIEnv * env, jobject obj)
{
	OPLog("RENDER");

	RenderSystem::ClearColor(1.0f, 0.0f, 0.0f);

	RenderSystem::UseMaterial(material);
	
	material->set_matrix(mvpLoc, &m[0][0]);

	tex->bind(sampLoc);


	if(firstRun){ 
		OPLog("Set Texture");
		OPLogNum(mvpLoc);
		OPLogNum(sampLoc);
		OPLogNum(buffer->handle());
		OPLogNum(uv->handle());
	}

		RenderSystem::SetBuffer(buffer->handle());
		material->set_data(0, 3, false, 0, (void*)0);
		material->enable_attrib(0);
		if(firstRun) OPLog("Set Vertex Buffer");
			
		RenderSystem::SetBuffer(uv->handle());
		material->set_data(1, 2, false, 0, (void*)0);
		material->enable_attrib(1);
		if(firstRun) OPLog("Set UV Buffer");

	RenderSystem::RenderTriangles(0, 12*3);
	if(firstRun) OPLog("Rendered Triangles");

	material->disable_attrib(0);
	material->disable_attrib(1);

	RenderSystem::Present();
	if(firstRun) OPLog("Presented");

	firstRun = false;
}

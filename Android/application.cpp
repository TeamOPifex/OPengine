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
#include <android/log.h>

//static const char gVertexShader[] = 
//    "attribute vec4 vPosition;\n"
//    "uniform vec4 MVP; \n"
//    "void main() {\n"
//    "  gl_Position = MVP * vPosition;\n"
//    "}\n";
//
//static const char gFragmentShader[] = 
//    "precision mediump float;\n"
//    "void main() {\n"
//    "  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
//    "}\n";

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
	"  gl_FragColor = texture2D(Texture, TexCoordOut) + vec4(0, TexCoordOut.r, TexCoordOut.g, 1);\n"
    "}\n";

//static const char gVertexShader[] = 
//    "layout(location = 0) in vec3 vertexPosition_modelspace; \n"
//	"layout(location = 1) in vec2 vertexUV;"
//	"out vec2 UV;\n"
//	"uniform mat4 MVP;\n"
//	"void main(){\n"
//    "gl_Position = MVP * vec4(vertexPosition_modelspace,1);\n"
//	"UV = vertexUV;\n"
//	"}";
//
//static const char gFragmentShader[] = 
//	"in vec2 UV; \n"
//	"out vec3 color; \n"
//	"uniform sampler2D myTextureSampler; \n"
//    "void main() { \n"
//	"  color = texture2D( myTextureSampler, UV ).rgb; \n"
//    "} \n";

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

int gltIsExtSupported(const char *extension)
	{
        GLubyte *extensions = NULL;
        const GLubyte *start;
        GLubyte *where, *terminator;
        
        where = (GLubyte *) strchr(extension, ' ');
        if (where || *extension == '\0')
            return 0;
        
        extensions = (GLubyte *)glGetString(GL_EXTENSIONS);
        
        start = extensions;
        for (;;) 
		{
            where = (GLubyte *) strstr((const char *) start, extension);
            
            if (!where)
                break;
            
            terminator = where + strlen(extension);
            
            if (where == start || *(where - 1) == ' ') 
			{
                if (*terminator == ' ' || *terminator == '\0') 
                    return 1;
			}
            start = terminator;
		}
		return 0;
	}

extern "C" {
    JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height, jobject assetManager);
    JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_step(JNIEnv * env, jobject obj);
};

JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height, jobject assetManager)
{
	OPLog("Is Texture format supported?");
	OPLogNum(gltIsExtSupported("GL_COLOR_EXT"));

	__android_log_print(ANDROID_LOG_ERROR, "OPIFEX", (const char*)glGetString(GL_EXTENSIONS));

	RenderSystem::Initialize(OpenGL_ES_2_0);

	Matrix4 v, p;
	m.SetIdentity();
	p = Matrix4::CreatePerspective(45.0f, 480.0f / 800.0f, 1.0f, 100.0f);
	v = Matrix4::CreateLook(Vector3(4,3,3), Vector3(0), Vector3(0,1,0));
	m = m * v * p;

	ShaderPtr vertex = new GLESShader(Vertex, gVertexShader);
	ShaderPtr pixel = new GLESShader(Fragment, gFragmentShader);

	material = new GLESMaterial(vertex, pixel);	
	mvpLoc = material->uniform_location("MVP");
	sampLoc = material->uniform_location("Texture");

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
		return;
	}

	fseek(myFile, start, SEEK_SET);
	TextureDDS* dds = new TextureDDS(myFile);
	if(!dds){
		OPLog("Texture not loaded.");
		return;
	}
	
	tex = new GLTexture(dds);
	delete(dds);

	buffer = new GLESBuffer(0, sizeof(gTriangleVertices), gTriangleVertices);

	uv = new GLESBuffer(0, sizeof(g_uv_buffer_data), g_uv_buffer_data);
	
	OPLog("Initialized Successfully");
}

bool firstRun = true;

JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_step(JNIEnv * env, jobject obj)
{
	RenderSystem::ClearColor(1.0f, 0.0f, 0.0f);

	RenderSystem::UseMaterial(material);
	
	material->set_matrix(mvpLoc, &m[0][0]);
	
	material->enable_attrib(sampLoc);
	tex->bind(sampLoc);

	if(firstRun){ 
		OPLog("Set Texture");
		OPLogNum(mvpLoc);
		OPLogNum(sampLoc);
		OPLogNum(buffer->handle());
		OPLogNum(uv->handle());
	}
	
	material->enable_attrib(0);
	RenderSystem::SetBuffer(buffer->handle());
	material->set_data(0, 3, false, 0, (void*)0);
	if(firstRun) OPLog("Set Vertex Buffer");
			
	material->enable_attrib(1);
	RenderSystem::SetBuffer(uv->handle());
	material->set_data(1, 2, false, 0, (void*)0);
	if(firstRun) OPLog("Set UV Buffer");

	RenderSystem::RenderTriangles(0, 12*3);
	if(firstRun) OPLog("Rendered Triangles");

	material->disable_attrib(0);
	material->disable_attrib(1);
	material->disable_attrib(sampLoc);

	RenderSystem::Present();
	if(firstRun) OPLog("Presented");

	firstRun = false;
}

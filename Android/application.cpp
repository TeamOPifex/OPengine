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

#include "Human\Resources\Sound\Sound.h"

#include "Human\Rendering\OBJLoader.h"

// Shaders
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

// Vertex & UV Data
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

// Global Variables

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
Matrix4 t1, t2;
bool updown;
int changes;

f32 r;
f32 g;
f32 b;
f32 translateX;
f32 translateZ;

bool firstRun = false;
Matrix4 result;
int offset;

GLBuffer* vertices;
GLBuffer* indices;
Mesh* mesh;

extern "C" {
	JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height, jobject assetManager);
	JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_setControllerButton(JNIEnv * env, jobject obj,  jint player,  jint button,  jint state);
    JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_setControllerAxes(JNIEnv * env, jobject obj,  jint player,  jint axes,  jfloat position);
    JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_step(JNIEnv * env, jobject obj);
};

class FileInformation {
public:
	FileInformation(FILE* _file, off_t _start, off_t _length, int fd) {
		file = _file;
		start = _start;
		length = _length;
		fileDescriptor = fd;
	}
	FILE* file;
	off_t start;
	off_t length;
	int fileDescriptor;
};

FileInformation* readFile(AAssetManager* mgr, char* filename){
	AAsset* asset = AAssetManager_open(mgr, filename, AASSET_MODE_UNKNOWN);
	if(asset == NULL)
		OPLog("Asset not loaded.");

	off_t _start, _length;
    int fd = AAsset_openFileDescriptor(asset, &_start, &_length);

    FILE* myFile = fdopen(dup(fd), "rb"); 
	if(!myFile){
		OPLog("File not loaded.");
	}
	fseek(myFile, _start, SEEK_SET);

	return new FileInformation(myFile, _start, _length, fd);
}

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
	Matrix4 r1 = Matrix4::RotateY(-0.025f);
	Matrix4 r2 = Matrix4::RotateX(-0.025f);
	t1 = Matrix4::Translate(0.01f, 0, 0);
	t2 = Matrix4::Translate(-0.01f, 0, 0);
	rotate = t1;

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
	
	FileInformation* fileInfo;

	fileInfo = readFile(mgr, "steamPlane.obj");
	mesh = LoadOBJ(fileInfo->file, fileInfo->start, fileInfo->length);

	vertices = new GLBuffer(1, sizeof(MeshVertex) * mesh->vertexCount, mesh->points);
	indices = new GLBuffer(2, sizeof(int) * mesh->indicesCount, mesh->indices);
	
	OPLog("Set Mesh Data");
	OPLog_i32(sizeof(Vector2));
	OPLog_i32(sizeof(Vector3));
	OPLog_i32(sizeof(MeshVertex));
	OPLog_i32(bufferLoc);
	OPLog_i32(uvLoc);
	OPLog("First Vertex");
	OPLog_f32(mesh->points[0].vertice._x);
	OPLog_f32(mesh->points[0].vertice._y);
	OPLog_f32(mesh->points[0].vertice._z);
	OPLog_f32(mesh->points[0].tex._x);
	OPLog_f32(mesh->points[0].tex._y);
	OPLog("First Triangle");
	OPLog_i32(mesh->indices[0]);
	OPLog_i32(mesh->indices[1]);
	OPLog_i32(mesh->indices[2]);
	
	void* off = 0;

	RenderSystem::SetBuffer(1, vertices->handle());
	material->set_data(bufferLoc, 3, false, sizeof(MeshVertex), 0);
	//off += 12;
	material->set_data(uvLoc, 2, false, sizeof(MeshVertex), (void*)(sizeof(f32) * 3));


	// TextureDDS Should take a stream
	fileInfo = readFile(mgr, "steamPlaneSkin.dds");
	//fileInfo = readFile(mgr, "bricks.dds");
	Texture* dds = new TextureDDS(fileInfo->file);	
	tex = new GLTexture(dds);
	delete(dds);
	
	//buffer = new GLBuffer(0, sizeof(f32) * 108, gTriangleVertices);
	//uv = new GLBuffer(0, sizeof(f32) * 72, g_uv_buffer_data);

	fileInfo = readFile(mgr, "AMemoryAway.ogg");
	Sound* snd = new Sound(fileInfo->fileDescriptor, fileInfo->start, fileInfo->length);

	OPLog("Initialized Successfully");
}

int get_int_by_JavaObj(JNIEnv* env, jobject java_obj, const char* field_name) {
	jclass clazz = env->GetObjectClass(java_obj);
	jfieldID int_fid = env->GetFieldID(clazz, field_name, "I");
	return env->GetIntField(java_obj, int_fid);
}

JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_setControllerButton(JNIEnv * env, jobject obj,  jint player,  jint button,  jint state){
	OPLog("Controller Button: ");
	OPLog_i32(player);
	OPLog_i32(button);
	OPLog_i32(state);
}

JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_setControllerAxes(JNIEnv * env, jobject obj,  jint player,  jint axes,  jfloat position){
	if(player == 1 && axes == 1){
		r = position;
	} else if(player == 1 && axes == 2){
		g = position;
	} else if(player == 1 && axes == 3){
		b = position;
	} else if(player == 2 && axes == 1){
		translateX = position;
	} else if(player == 2 && axes == 2){
		translateZ = position;
	}
}

JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_step(JNIEnv * env, jobject obj){
	RenderSystem::ClearColor(r, g, b);

	// Set material to use
	RenderSystem::UseMaterial(material);
	
	// Set MVP Matrix	
	m = Matrix4::Translate(translateX, 0, translateZ);
	result = m * v * p;
	material->set_matrix(mvpLoc, &result[0][0]);
	
	// Set Texture
	material->enable_attrib(sampLoc);
	tex->bind(sampLoc);
	

	//// Set Vertex Buffer
	//material->enable_attrib(bufferLoc);
	//RenderSystem::SetBuffer(buffer->handle());
	//// attribute, size, normalize, stride, arrayData
	//material->set_data(bufferLoc, 3, false, 0, (void*)offset);
	//		
	//// Set UV Buffer
	//material->enable_attrib(uvLoc);
	//RenderSystem::SetBuffer(uv->handle());
	//material->set_data(uvLoc, 2, false, 0, (void*)offset);

	//if(!firstRun){
	//	OPLog_i32(buffer->handle());
	//	OPLog_i32(uv->handle());
	//}

	// Render the scene

	material->enable_attrib(bufferLoc);
	material->enable_attrib(uvLoc);
	//RenderSystem::SetBuffer(1, vertices->handle());
	RenderSystem::SetBuffer(2, indices->handle());
	RenderSystem::RenderTriangles(mesh->indicesCount);	
	RenderSystem::Present();

	// Clean up
	material->disable_attrib(bufferLoc);
	material->disable_attrib(uvLoc);
	material->disable_attrib(sampLoc);

	firstRun = true;
}

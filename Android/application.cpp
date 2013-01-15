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

Matrix4 t3, t4, t5, t6;
JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height, jobject assetManager)
{	
	AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);
	if(mgr == NULL)
		OPLog("Asset manager not created.");
	OPfileInit(mgr);

	RenderSystem::Initialize();

	m.SetIdentity();
	p = Matrix4::CreatePerspective(45.0f, 1280.0f / 800.0f, 1.0f, 100.0f);
	v = Matrix4::CreateLook(Vector3(-16,3,12), Vector3(0), Vector3(0,1,0));
	Matrix4 r1 = Matrix4::RotateY(-0.025f);
	Matrix4 r2 = Matrix4::RotateX(-0.025f);
	t1 = Matrix4::Translate(0.01f, 0, 0);
	t2 = Matrix4::Translate(-0.01f, 0, 0);
	
	f32 offset = 20.0f;
	t3 = Matrix4::Translate(-offset, 0, 0);
	t4 = Matrix4::Translate(offset, 0, 0);
	t5 = Matrix4::Translate(0, 0, -offset);
	t6 = Matrix4::Translate(0, 0, offset);
	rotate = t1;

	ShaderPtr vertex = new GLShader(Vertex, gVertexShader);
	ShaderPtr pixel = new GLShader(Fragment, gFragmentShader);

	material = new GLMaterial(vertex, pixel);	
	mvpLoc = material->uniform_location("MVP");
	sampLoc = material->uniform_location("Texture");
	bufferLoc = material->attribute_location("vPosition");
	uvLoc = material->attribute_location("TexCoordIn");
	

	FileInformation* fileInfo = readFile(mgr, "steamPlane.obj");
	mesh = LoadOBJ(fileInfo->file, fileInfo->start, fileInfo->length);

	vertices = new GLBuffer(1, sizeof(MeshVertex) * mesh->vertexCount, mesh->points);
	indices = new GLBuffer(2, sizeof(int) * mesh->indicesCount, mesh->indices);
		
	material->set_data(bufferLoc, 3, false, sizeof(MeshVertex), 0);
	material->set_data(uvLoc, 2, false, sizeof(MeshVertex), (void*)(sizeof(f32) * 3));


	// TextureDDS Should take a stream
	fileInfo = readFile(mgr, "steamPlaneSkin.dds");
	Texture* dds = new TextureDDS(fileInfo->file);	
	tex = new GLTexture(dds);
	delete(dds);

	fileInfo = readFile(mgr, "AMemoryAway.ogg");
	Sound* snd = new Sound(fileInfo->fileDescriptor, fileInfo->start, fileInfo->length);
	
	RenderSystem::UseMaterial(material);
	translateX = 0;
	translateZ = 0;
	m = Matrix4::Translate(translateX, 0, translateZ);
	result = m * v * p;
	material->set_matrix(mvpLoc, &result[0][0]);
	
	// Set Texture
	material->enable_attrib(sampLoc);
	tex->bind(sampLoc);	
	material->enable_attrib(bufferLoc);
	material->enable_attrib(uvLoc);

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
f32 x_move;
f32 z_move;

JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_setControllerAxes(JNIEnv * env, jobject obj,  jint player,  jint axes,  jfloat position){
	if(player == 1 && axes == 1){
		r = position;
	} else if(player == 1 && axes == 2){
		g = position;
	} else if(player == 1 && axes == 3){
		x_move = position;
	} else if(player == 1 && axes == 4){
		z_move = position;
	}
}

JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_step(JNIEnv * env, jobject obj){
	RenderSystem::ClearColor(r, g, b);

	// Set material to use
	RenderSystem::UseMaterial(material);
		
	// Render the scene
	RenderSystem::SetBuffer(2, indices->handle());
	
	translateX += x_move;
	translateZ += z_move;
	m = Matrix4::Translate(translateX, 0, translateZ);

	// Set MVP Matrix	
	result = m * v * p;
	material->set_matrix(mvpLoc, &result[0][0]);
	RenderSystem::RenderTriangles(mesh->indicesCount);	
	
	result = t3 * m * v * p;
	material->set_matrix(mvpLoc, &result[0][0]);
	RenderSystem::RenderTriangles(mesh->indicesCount);	

	result = t4 * m * v * p;
	material->set_matrix(mvpLoc, &result[0][0]);
	RenderSystem::RenderTriangles(mesh->indicesCount);	

	result = t5 * m * v * p;
	material->set_matrix(mvpLoc, &result[0][0]);
	RenderSystem::RenderTriangles(mesh->indicesCount);	

	result = t6 * m * v * p;
	material->set_matrix(mvpLoc, &result[0][0]);
	RenderSystem::RenderTriangles(mesh->indicesCount);	

	RenderSystem::Present();

	// Clean up
	//material->disable_attrib(bufferLoc);
	//material->disable_attrib(uvLoc);
	//material->disable_attrib(sampLoc);
}

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
#include <string.h>

// Global Variables

GLWorldMaterial* material;
GLTexture* tex;
GLTexture* texSpec;
GLTexture* texNorm;
ui32 sampLoc;
ui32 specLoc;
ui32 normLoc;
BufferPtr buffer;
BufferPtr uv;
ui32 mLoc;
ui32 vpLoc;
Matrix4 m;
ui32 bufferLoc;
ui32 uvLoc, normalLoc, tangentLoc;
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
Model* model;
f32 rotateAmnt;
f32 rotateAmnt2;
f32 x_move;
f32 z_move;
f32 r_move;
f32 r_move2;
Matrix4 rotating;
Matrix4 rotating2;
Matrix4 modelMatrix;
Matrix4 translateMatrix;

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
	p = Matrix4::CreatePerspective(45.0f, 1280.0f / 800.0f, 1.0f, 1000.0f);
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

	
	FileInformation* fileInfo = readFile(mgr, "vertex.fx");
	char* gVertexShader = (char*)OPalloc(sizeof(char) * fileInfo->length);
	fread(gVertexShader, 1, fileInfo->length, fileInfo->file);
	gVertexShader[fileInfo->length - 1] = '\0';
	ShaderPtr vertex = new GLShader(Vertex, gVertexShader);
	OPfree(gVertexShader);
	OPfree(fileInfo);

	
	fileInfo = readFile(mgr, "fragment.fx");
	char* gFragmentShader = (char*)OPalloc(fileInfo->length);
	fread(gFragmentShader, 1, fileInfo->length, fileInfo->file);
	gFragmentShader[fileInfo->length - 1] = '\0';
	ShaderPtr pixel = new GLShader(Fragment, gFragmentShader);
	OPfree(gFragmentShader);
	OPfree(fileInfo);
	
	material = new GLWorldMaterial(vertex, pixel);


	//material = new GLMaterial(vertex, pixel);	
	//mLoc = material->uniform_location("Model");
	vpLoc = material->uniform_location("ViewProjection");

	sampLoc = material->uniform_location("Texture");
	specLoc = material->uniform_location("SpecularTexture");
	normLoc = material->uniform_location("NormalTexture");
	
	bufferLoc = material->attribute_location("vPosition");
	normalLoc = material->attribute_location("vNormal");
	tangentLoc = material->attribute_location("vTangent");
	uvLoc = material->attribute_location("TexCoordIn");
	

	fileInfo = readFile(mgr, "steamPlane.obj");
	mesh = LoadOBJ(fileInfo->file, fileInfo->start, fileInfo->length);
	
	model = new Model(mesh, material, &m);

	//vertices = new GLBuffer(1, sizeof(MeshVertex) * mesh->vertexCount, mesh->points);
	//indices = new GLBuffer(2, sizeof(int) * mesh->indicesCount, mesh->indices);
	
	void* offsetBuffer = 0;
	material->set_data(bufferLoc, 3, false, sizeof(MeshVertex), offsetBuffer);
	offsetBuffer += 3 * sizeof(f32);
	material->set_data(uvLoc, 2, false, sizeof(MeshVertex), offsetBuffer);
	offsetBuffer += 2 * sizeof(f32);
	material->set_data(normalLoc, 3, false, sizeof(MeshVertex), offsetBuffer);
	offsetBuffer += 3 * sizeof(f32);
	material->set_data(tangentLoc, 3, false, sizeof(MeshVertex), offsetBuffer);


	// TextureDDS Should take a stream
	fileInfo = readFile(mgr, "steamPlaneSkin.dds");
	Texture* dds = new TextureDDS(fileInfo->file);	
	tex = new GLTexture(dds);
	delete(dds);
	OPfree(fileInfo);

	
	fileInfo = readFile(mgr, "steamPlaneSpec.dds");
	Texture* dds2 = new TextureDDS(fileInfo->file);	
	texSpec = new GLTexture(dds2);
	delete(dds2);
	OPfree(fileInfo);

	fileInfo = readFile(mgr, "normalMap.dds");
	Texture* dds3 = new TextureDDS(fileInfo->file);	
	texNorm = new GLTexture(dds3);
	delete(dds3);
	OPfree(fileInfo);

	fileInfo = readFile(mgr, "AMemoryAway.ogg");
	Sound* snd = new Sound(fileInfo->fileDescriptor, fileInfo->start, fileInfo->length);
	OPfree(fileInfo);
	
	RenderSystem::UseMaterial(material);
	translateX = 0;
	translateZ = 0;
	m = Matrix4::Translate(translateX, 0, translateZ);
	result = v * p;
	material->set_matrix(mLoc, &m[0][0]);
	material->set_matrix(vpLoc, &result[0][0]);
	
	// Set Texture
	material->enable_attrib(mLoc);
	material->enable_attrib(vpLoc);
	material->enable_attrib(sampLoc);
	tex->bind(sampLoc, 0);
	
	material->enable_attrib(specLoc);
	texSpec->bind(specLoc, 1);

	material->enable_attrib(normLoc);
	texNorm->bind(normLoc, 2);
	
	material->enable_attrib(bufferLoc);
	material->enable_attrib(normalLoc);
	material->enable_attrib(tangentLoc);
	material->enable_attrib(uvLoc);
	
	rotateAmnt = 0;
	rotateAmnt2 = 0;

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
	if(position < 0.05f && position > -0.05f)
		position = 0.0f;
		
	if(player == 1 && axes == 1){
		r_move = position * 0.01f;
	} else if(player == 1 && axes == 2){
		r_move2 = position * 0.01f;
	} else if(player == 1 && axes == 3){
		x_move = position;
	} else if(player == 1 && axes == 4){
		z_move = position;
	}
}


JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_step(JNIEnv * env, jobject obj){
	RenderSystem::ClearColor(r, g, b);

	translateX += x_move;
	translateZ += z_move;
	rotateAmnt += r_move;
	rotateAmnt2 += r_move2;
	
	rotating = Matrix4::RotateY(rotateAmnt);
	rotating2 = Matrix4::RotateX(rotateAmnt2);
	m = rotating * rotating2;
	model->WorldMatrix = &m;

	result = v * p;
	material->set_matrix(vpLoc, &result[0][0]);
	
	RenderSystem::RenderModel(model);

	RenderSystem::Present();
}

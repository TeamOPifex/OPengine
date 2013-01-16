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
    "attribute vec3 vNormal; \n"
    "attribute vec3 vTangent; \n"
    "attribute vec2 TexCoordIn; \n"
	
    "uniform mat4 Model; \n"
    "uniform mat4 ViewProjection; \n"
	
    "varying vec2 TexCoordOut; \n"
    "varying vec3 TangentOut; \n"
    "varying vec3 NormalOut; \n"

    "void main() {\n"
	" mat3 rotScl = mat3(Model[0].xyz, Model[1].xyz, Model[2].xyz); \n"
	"	vec4 worldPos = Model * vec4(vPosition,1);\n"
	"  gl_Position = ViewProjection * worldPos; \n"
    "  TexCoordOut = TexCoordIn; \n"
	"  NormalOut = normalize(rotScl * vNormal); \n"
	"  TangentOut = normalize(rotScl * vTangent); \n"
    "}\n";

static const char gFragmentShader[] = 
	"varying highp vec2 TexCoordOut; \n"
	"varying highp vec3 NormalOut; \n"
	"varying lowp vec3 TangentOut; \n"

	"uniform sampler2D Texture; \n"
	"uniform sampler2D SpecularTexture; \n"
	"uniform sampler2D NormalTexture; \n"

    "void main() {\n"
	"	mediump vec3 lightPos = vec3(1, -1, -2);\n"
	"	lowp vec3 lightDir = normalize(-lightPos);\n"

	"	lowp vec3 halfVec = normalize(vec3(0, 0, 1) + lightDir);\n"
	
	"	lowp vec3 biNormal = normalize(cross(NormalOut, TangentOut));\n"
	"	lowp mat3 tanSpace = mat3(TangentOut, biNormal, NormalOut);\n"
	"	lowp vec3 normal = normalize(tanSpace * (texture2D(NormalTexture, TexCoordOut) * 2.0 - 1.0).xyz);\n"
	
	"	lowp vec4 color = texture2D(Texture, TexCoordOut);\n"
	"	lowp vec4 diffuse = vec4(vec3(dot(lightDir, normal)), 1);\n"
	"	lowp vec4 specular = texture2D(SpecularTexture, TexCoordOut) * pow(dot(halfVec, normal), 64.0);\n"

	"   gl_FragColor = vec4((color.xyz * diffuse.xyz) + specular.xyz, 1);\n"
    "}\n";

// Global Variables

MaterialPtr material;
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

f32 rotateAmnt;
f32 rotateAmnt2;

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

	ShaderPtr vertex = new GLShader(Vertex, gVertexShader);
	ShaderPtr pixel = new GLShader(Fragment, gFragmentShader);

	material = new GLMaterial(vertex, pixel);	
	mLoc = material->uniform_location("Model");
	vpLoc = material->uniform_location("ViewProjection");

	sampLoc = material->uniform_location("Texture");
	specLoc = material->uniform_location("SpecularTexture");
	normLoc = material->uniform_location("NormalTexture");
	
	bufferLoc = material->attribute_location("vPosition");
	normalLoc = material->attribute_location("vNormal");
	tangentLoc = material->attribute_location("vTangent");
	uvLoc = material->attribute_location("TexCoordIn");
	

	FileInformation* fileInfo = readFile(mgr, "steamPlane.obj");
	mesh = LoadOBJ(fileInfo->file, fileInfo->start, fileInfo->length);

	vertices = new GLBuffer(1, sizeof(MeshVertex) * mesh->vertexCount, mesh->points);
	indices = new GLBuffer(2, sizeof(int) * mesh->indicesCount, mesh->indices);
	
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

	
	fileInfo = readFile(mgr, "steamPlaneSpec.dds");
	Texture* dds2 = new TextureDDS(fileInfo->file);	
	texSpec = new GLTexture(dds2);
	delete(dds2);

	fileInfo = readFile(mgr, "normalMap.dds");
	Texture* dds3 = new TextureDDS(fileInfo->file);	
	texNorm = new GLTexture(dds3);
	delete(dds3);

	fileInfo = readFile(mgr, "AMemoryAway.ogg");
	Sound* snd = new Sound(fileInfo->fileDescriptor, fileInfo->start, fileInfo->length);
	
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
f32 x_move;
f32 z_move;
f32 r_move;
f32 r_move2;

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
Matrix4 rotating;
Matrix4 rotating2;
Matrix4 modelMatrix;
Matrix4 translateMatrix;


JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_step(JNIEnv * env, jobject obj){
	RenderSystem::ClearColor(r, g, b);

	// Set material to use
	RenderSystem::UseMaterial(material);
		
	// Render the scene
	RenderSystem::SetBuffer(2, indices->handle());
	
	translateX += x_move;
	translateZ += z_move;
	rotateAmnt += r_move;
	rotateAmnt2 += r_move2;
	
	rotating = Matrix4::RotateY(rotateAmnt); 
	rotating2 = Matrix4::RotateX(rotateAmnt2); 
	//m = Matrix4::Translate(translateX, 0, translateZ);
	m = rotating * rotating2;

	//tex->bind(sampLoc, 0);	
	//texSpec->bind(specLoc, 1);

	// Set MVP Matrix	
	result = v * p;
	material->set_matrix(vpLoc, &result[0][0]);

	modelMatrix = m;
	material->set_matrix(mLoc, &modelMatrix[0][0]);
	RenderSystem::RenderTriangles(mesh->indicesCount);	
	
	modelMatrix = t3 * m;
	material->set_matrix(mLoc, &modelMatrix[0][0]);
	RenderSystem::RenderTriangles(mesh->indicesCount);	

	modelMatrix = t4 * m;
	material->set_matrix(mLoc, &modelMatrix[0][0]);
	RenderSystem::RenderTriangles(mesh->indicesCount);	

	modelMatrix = t5 * m;
	material->set_matrix(mLoc, &modelMatrix[0][0]);
	RenderSystem::RenderTriangles(mesh->indicesCount);

	modelMatrix = t6 * m;
	material->set_matrix(mLoc, &modelMatrix[0][0]);
	RenderSystem::RenderTriangles(mesh->indicesCount);	

	
	
	modelMatrix = t6 * t4 * m;
	material->set_matrix(mLoc, &modelMatrix[0][0]);
	RenderSystem::RenderTriangles(mesh->indicesCount);	

	modelMatrix = t4 * t5 * m;
	material->set_matrix(mLoc, &modelMatrix[0][0]);
	RenderSystem::RenderTriangles(mesh->indicesCount);	

	modelMatrix = t5 * t3 * m;
	material->set_matrix(mLoc, &modelMatrix[0][0]);
	RenderSystem::RenderTriangles(mesh->indicesCount);

	modelMatrix = t6 * t3 * m;
	material->set_matrix(mLoc, &modelMatrix[0][0]);
	RenderSystem::RenderTriangles(mesh->indicesCount);	

	RenderSystem::Present();

	// Clean up
	//material->disable_attrib(bufferLoc);
	//material->disable_attrib(uvLoc);
	//material->disable_attrib(sampLoc);
}

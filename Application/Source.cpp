#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./Core/include/Log.h"
#include "./Data/include/OPlinkedList.h"
#include "./Data/include/OPheap.h"
#include "./Data/include/OPfile.h"
#include "./Human/Rendering/RenderSystem.h"
//#include "./Human/Resources/Texture/TextureDDS.h"
//#include "./Human/Rendering/GL/GLTexture.h"
//#include "./Human/Rendering/GL/GLShader.h"
//#include "./Human/Rendering/GL/GLMaterial.h"
//#include "./Human/Rendering/GL/GLBuffer.h"
#include "./Human/Math/Matrix4.h"
#include "./Human/Resources/Sound/Sound.h"
//#include "./Human/Rendering/OBJLoader.h"
//#include "./Human/Rendering/GL/GLUtility.h"
//#include "./Human/Rendering/GL/GLTexturedMaterial.h"
//#include "./Human/Rendering/GL/GLWorldTexturedMaterial.h"
//#include "./Human/Rendering/GL/GLWorldTexturedSpecularMaterial.h"

#include "./Human/Input/GamePadSystem.h"
#include "./Human/Audio/Jukebox.h"
#include "./Human/Rendering/Quad.h"

#ifdef OPIFEX_ANDROID

#include <jni.h>

#include <sys/types.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>

#include <unistd.h>

#else

#include <direct.h>

#endif


#ifdef OPIFEX_ANDROID
// BELOW - Still working on Windows File Reading support. Android works.
#else
static const char gVertexShader[] = 
	"#version 330 core\n"
    "in vec3 vPosition; \n"
    "in vec3 vNormal; \n"
    "in vec3 vTangent; \n"
	"in vec2 TexCoordIn;\n"
	"uniform mat4 Model;\n"
	"uniform mat4 ViewProjection;\n"
	
	"out vec2 TexCoordOut;\n"
	"out vec3 TangentOut;\n"
	"out vec3 NormalOut;\n"

	"void main(){\n"
		"mat3 rotScl = mat3(Model[0].xyz, Model[1].xyz, Model[2].xyz); \n"
		"vec4 worldPos = Model * vec4(vPosition,1);\n"
		"gl_Position = ViewProjection * worldPos; \n"
		"TexCoordOut = TexCoordIn;  \n"
		"NormalOut = normalize(rotScl * vNormal); \n"
		"TangentOut = normalize(rotScl * vTangent); \n"
	"}";

static const char gFragmentShader[] = 
	"#version 330 core\n"
	"in vec2 TexCoordOut; \n"
	"in vec3 NormalOut; \n"
	"in vec3 TangentOut; \n"
	"uniform sampler2D Texture; \n"
	"uniform sampler2D SpecularTexture; \n"
	"uniform sampler2D NormalTexture; \n"
    "void main() { \n"
	"	vec3 lightPos = vec3(1, -1, -2);\n"
	"	vec3 lightDir = normalize(-lightPos);\n"
	"	vec3 halfVec = normalize(vec3(0, 0, 1) + lightDir);\n"
	"	vec3 biNormal = normalize(cross(NormalOut, TangentOut));\n"
	"	mat3 tanSpace = mat3(TangentOut, biNormal, NormalOut);\n"
	"	vec3 normal = normalize(tanSpace * (texture2D(NormalTexture, TexCoordOut) * 2.0 - 1.0).xyz);\n"
	"	vec4 color = texture2D(Texture, TexCoordOut);\n"
	"	vec4 diffuse = vec4(vec3(dot(lightDir, normal)), 1);\n"
	"	vec4 specular = texture2D(SpecularTexture, TexCoordOut) * pow(dot(halfVec, normal), 64.0);\n"
	"  gl_FragColor = vec4((color.xyz * diffuse.xyz) + specular.xyz, 1); \n"
    "} \n";
#endif

// Global Variables

//GLWorldTexturedSpecularMaterial* material;
//GLWorldTexturedMaterial* material2;
//GLTexturedMaterial* material3;
//GLTexture* tex;
//GLTexture* texSpec;
//GLTexture* texNorm;
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
//GLBuffer* vertices;
//GLBuffer* indices;
Mesh* mesh;
//ModelWorld* model;
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
Matrix4 t3, t4, t5, t6;
Audio* audio;
Audio* audio2;

#ifdef OPIFEX_ANDROID
AAssetManager* mgr;
#endif
//
//class FileInformation {
//public:
//	FileInformation(FILE* _file, ui32 _start, ui32 _length, int fd) {
//		file = _file;
//		start = _start;
//		length = _length;
//		fileDescriptor = fd;
//	}
//	FILE* file;
//	ui32 start;
//	ui32 length;
//	int fileDescriptor;
//};

#ifdef OPIFEX_ANDROID

extern "C" {
	JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height, jobject assetManager);
	JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_setControllerButton(JNIEnv * env, jobject obj,  jint player,  jint button,  jint state);
    JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_setControllerAxes(JNIEnv * env, jobject obj,  jint player,  jint axes,  jfloat position);
    JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_step(JNIEnv * env, jobject obj);
};

//FileInformation* readFile(char* filename){
//	AAsset* asset = AAssetManager_open(mgr, filename, AASSET_MODE_UNKNOWN);
//	if(asset == NULL)
//		OPLog("Asset not loaded.");
//
//	off_t _start, _length;
//    int fd = AAsset_openFileDescriptor(asset, &_start, &_length);
//
//    FILE* myFile = fdopen(dup(fd), "rb"); 
//	if(!myFile){
//		OPLog("File not loaded.");
//	}
//	fseek(myFile, _start, SEEK_SET);
//
//	return new FileInformation(myFile, _start, _length, fd);
//}
#else
//FileInformation* readFile(char* filename){
//    FILE* myFile = fopen(filename, "rb"); 
//	if(!myFile){
//		OPLog("File not loaded.");
//	}
//	fseek(myFile, 0, SEEK_END );
//	int Size = ftell( myFile );
//	fseek(myFile, 0, SEEK_SET);
//
//	return new FileInformation(myFile, 0, Size, 0);
//}
#endif


#ifdef OPIFEX_ANDROID
JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height, jobject assetManager){
	mgr = AAssetManager_fromJava(env, assetManager);

	if(mgr == NULL)
		OPLog("Asset manager not created.");
	OPfileInit(mgr);
#else
int main(){	
   _chdir("Assets\\");
	//char* buffer;

 //  // Get the current working directory: 
 //  if( (buffer = _getcwd( NULL, 0 )) == NULL )
 //     perror( "_getcwd error" );
 //  else
 //  {
 //     printf( "%s \nLength: %d\n", buffer, strlen(buffer) );
 //     free(buffer);
 //  }

#endif

	//RenderSystem::Initialize();

	if(Jukebox::Initialize()){
		audio = Jukebox::Load("AMemoryAway.ogg", false);
		Jukebox::Play(audio);
		audio->SetVolume(-2000);
		audio2 = Jukebox::Load("Plane_Normal_Throttle.ogg", true);
		Jukebox::Play(audio2);
		audio2->SetVolume(-800);
	}

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

	
#ifdef OPIFEX_ANDROID
	//FileInformation fileInfo = OPreadFile_Android("vertex.fx");
	//FileInformation fileInfo = OPreadFile_Android("vertex2.fx");
	FileInformation fileInfo = OPreadFile_Android("vertex3.fx");
	char* gVertexShader = (char*)OPalloc(sizeof(char) * fileInfo.length);
	fread(gVertexShader, 1, fileInfo.length, fileInfo.file);
	gVertexShader[fileInfo.length - 1] = '\0';
	ShaderPtr vertex = new GLShader(Vertex, gVertexShader);
	OPfree(gVertexShader);
	OPfree(&fileInfo);

	
	//fileInfo = OPreadFile_Android("fragment.fx");
	fileInfo = OPreadFile_Android("fragment2.fx");
	char* gFragmentShader = (char*)OPalloc(fileInfo.length);
	fread(gFragmentShader, 1, fileInfo.length, fileInfo.file);
	gFragmentShader[fileInfo.length - 1] = '\0';
	OPLog(gFragmentShader);
	ShaderPtr pixel = new GLShader(Fragment, gFragmentShader);
	OPfree(gFragmentShader);
	OPfree(&fileInfo);
#else
	////FileInformation* fileInfo;// = readFile("vertex.fx");
	////char* gVertexShader = (char*)OPalloc(sizeof(char) * fileInfo->length);
	////fread(gVertexShader, 1, fileInfo->length, fileInfo->file);
	////gVertexShader[fileInfo->length - 1] = '\0';
	//ShaderPtr vertex = new GLShader(Vertex, gVertexShader);
	////OPfree(gVertexShader);
	////OPfree(fileInfo);

	
	////fileInfo = readFile("fragment.fx");
	////char* gFragmentShader = (char*)OPalloc(fileInfo->length);
	////fread(gFragmentShader, 1, fileInfo->length, fileInfo->file);
	////gFragmentShader[fileInfo->length - 1] = '\0';
	////OPLog(gFragmentShader);
	//ShaderPtr pixel = new GLShader(Fragment, gFragmentShader);
	////OPfree(gFragmentShader);
	////OPfree(fileInfo);
#endif
	
	////material = new GLWorldTexturedSpecularMaterial(vertex, pixel);
	////material2 = new GLWorldTexturedMaterial(vertex, pixel);
	//material3 = new GLTexturedMaterial(vertex, pixel);

	////vpLoc = material->uniform_location("ViewProjection");

	////sampLoc = material->uniform_location("Texture");
	////specLoc = material->uniform_location("SpecularTexture");
	////normLoc = material->uniform_location("NormalTexture");
	//
	////OPLog_i32(sampLoc);
	////OPLog_i32(specLoc);
	////OPLog_i32(normLoc);
	//
	////bufferLoc = material->attribute_location("vPosition");
	////normalLoc = material->attribute_location("vNormal");
	////tangentLoc = material->attribute_location("vTangent");
	////uvLoc = material->attribute_location("TexCoordIn");
	//

	////fileInfo = OPreadFile_Android("steamPlane.obj");
	////mesh = LoadOBJ(fileInfo.file, fileInfo.start, fileInfo.length);	
	////model = new Model(mesh, material);
	//
	////model = new Quad(material2);
	//model = new Quad(material3);

	//GLUtility::CheckError("Application::Clear Errors");

	OPlinkedList* ll = OPllCreate();

	printf("Inserting ");
	for(OPint i = 10; i--;){
		OPint* j = (OPint*)OPalloc(sizeof(OPint));
		*j = i;
		OPllInsertLast(ll, (ui8*)j);
		printf("%d ", *j);
	}
	printf("\n");
	
	printf("Reading: ");
	OPllNode* node = ll->First;
	while (node)
	{
		printf("%d ", *(OPint*)(node->Data));
		//node = node->Next;
	}
	printf("\n");
	

	//material->enable_attrib(bufferLoc);
	//material->enable_attrib(normalLoc);
	//material->enable_attrib(tangentLoc);
	//material->enable_attrib(uvLoc);
	
	//RenderSystem::SetBuffer(1, model->ModelMesh->VertexBuffer->handle());
	//
	////ui32 offsetBuffer = 0;
	//material->set_data(bufferLoc, 3, false, sizeof(MeshVertexTextured), (void*)0);
	////offsetBuffer += (3 * sizeof(f32));
	//material->set_data(uvLoc, 2, false, sizeof(MeshVertexTextured), (void*)12);
	////offsetBuffer += 2 * sizeof(f32);
	//material->set_data(normalLoc, 3, false, sizeof(MeshVertexTextured), (void*)20);
	////offsetBuffer += 3 * sizeof(f32);
	//material->set_data(tangentLoc, 3, false, sizeof(MeshVertexTextured), (void*)32);


	// TextureDDS Should take a stream
	/*fileInfo = OPreadFile_Android("steamPlaneSkin.dds");
	Texture* dds = new TextureDDS(fileInfo.file);	
	tex = new GLTexture(dds);
	delete(dds);
	OPfree(&fileInfo);
	*/
	
	//fileInfo = OPreadFile_Android("steamPlaneSpec.dds");
	//Texture* dds2 = new TextureDDS(fileInfo.file);	
	//texSpec = new GLTexture(dds2);
	//delete(dds2);
	//OPfree(&fileInfo);

	//fileInfo = OPreadFile_Android("normalMap.dds");
	//Texture* dds3 = new TextureDDS(fileInfo.file);	
	//texNorm = new GLTexture(dds3);
	//delete(dds3);
	//OPfree(&fileInfo);

#ifdef OPIFEX_ANDROID
	//fileInfo = OPreadFile_Android("AMemoryAway.ogg");
	//Sound* snd = new Sound(fileInfo.fileDescriptor, fileInfo.start, fileInfo.length);
	//OPfree(&fileInfo);
#else
	// Windows/Linux sound not implemented yet
#endif
//	//RenderSystem::UseMaterial(material);	
//	//RenderSystem::UseMaterial(material2);
//	RenderSystem::UseMaterial(material3);
//
//	translateX = 0;
//	translateZ = 0;
//	m = Matrix4::Translate(translateX, 0, translateZ);
//	result = v * p;
//	//material->set_matrix(mLoc, &m[0][0]);
//	//material->set_matrix(vpLoc, &result[0][0]);
//		
///*	material2->EnableAttributes();
//	material2->SetWorldMatrix(&m[0][0]);
//	material2->SetViewProjectionMatrix(&result[0][0]);
//	material2->SetTexture(tex, 0);*/	
//	
//	material3->EnableAttributes();
//	material3->SetWorldMatrix(&m[0][0]);
//	//material3->SetTexture(tex, 0);
//
//	//material->EnableAttributes();
//	//material->SetWorldMatrix(&m[0][0]);
//	//material->SetViewProjectionMatrix(&result[0][0]);
//	//material->SetTexture(tex, 0);
//	//material->SetTextureSpecular(texSpec, 1);
//	//material->SetTextureNormal(texNorm, 2);
//
//	//material->enable_attrib(mLoc);
//	//material->enable_attrib(vpLoc);
//	//material->enable_attrib(sampLoc);
//	//tex->bind(sampLoc, 0);
//	
//	//material->enable_attrib(specLoc);
//	//texSpec->bind(specLoc, 1);
//
//	//material->enable_attrib(normLoc);
//	//texNorm->bind(normLoc, 2);
//	//
//	//material->enable_attrib(bufferLoc);
//	//material->enable_attrib(normalLoc);
//	//material->enable_attrib(tangentLoc);
//	//material->enable_attrib(uvLoc);
	
	rotateAmnt = 0;
	rotateAmnt2 = 0;

	OPLog("Initialized Successfully");
#ifdef OPIFEX_ANDROID
	}
	

JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_setControllerButton(JNIEnv * env, jobject obj,  jint player,  jint button,  jint state){
	GamePadSystem::Controller((GamePadIndex)player)->SetButton((GamePadButton)button, state == 1);
}


JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_setControllerAxes(JNIEnv * env, jobject obj,  jint player,  jint axes,  jfloat position){
	GamePadSystem::Controller((GamePadIndex)(player))->SetAxis((GamePadAxes)axes, position);
}


JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_step(JNIEnv * env, jobject obj){
	GamePadSystem::Update();

#else
	do{
#endif
		RenderSystem::ClearColor(r, g, b);

		translateX += x_move;
		translateZ += z_move;
		rotateAmnt += r_move;
		rotateAmnt2 += r_move2;
	
		rotating = Matrix4::RotateY(rotateAmnt);
		rotating2 = Matrix4::Scale(2.0f);
		m = rotating * rotating2;
		//model->WorldMatrix = &m;

		//result = v * p;
		//material->set_matrix(vpLoc, &result[0][0]);
	
		//RenderSystem::RenderModel(model);

		RenderSystem::Present();
#ifdef OPIFEX_ANDROID

#else
	}
	while(RenderSystem::escape());
	
	printf("Program Ended.");

	return 0;
#endif
}

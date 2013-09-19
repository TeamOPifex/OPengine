#include "GameManager.h"
#include "./Human/include/Utilities/OBJLoader.h"
#include "./Data/include/OPfile.h"
#include "./Data/include/OPcontentManager.h"
#include "./Math/include/Matrix4.h"
#include "./Human/include/Resources/Texture/ImageDDS.h"
#include "./Human/include/Resources/Texture/ImagePNG.h"
#include "./Human/include/Utilities/Errors.h"
//#include "./Human/include/Utilities/OPMLoader.h"
#include "./Human/include/Rendering/Buffer.h"
#include "./Human/include/Rendering/OPmesh.h"
#include "./Human/include/Rendering/OPmeshPacked.h"
#include "./Human/include/Rendering/OPmeshPacker.h"
#include "./Human/include/Rendering/OPeffect.h"
#include "./Human/include/Rendering/Camera.h"
#include "./Core/include/Log.h"

OPfloat vertData[] = {
	 0.5,  0.5, 0,
	   1,  0,   1,
	-0.5,  0.5, 0,
	   1,  0,   0,
	-0.5, -0.5, 0,
	   0,  1,   0,
	 0.5, -0.5, 0,
	   0,  0,   1
};

ui16 indexData[] = {
	0, 2, 3, 
	2, 0, 1
};

OPmeshPacked quad;
OPmeshPacker packer;
OPmesh* plane;
OPeffect tri;
OPcam camera;

GameManager::GameManager(int width, int height) 
{
	rotateAmnt = 0;
	OPrenderInit(width, height);

	OPcmanLoad("TexturedSpecular.vert");
	OPcmanLoad("TexturedSpecular.frag");
	OPcmanLoad("BiPlane.opm");

	packer = OPmeshPackerCreate();

	OPmeshPackerBind(&packer);
	quad = OPrenderCreateMeshPacked(
		sizeof(OPfloat) * 6, sizeof(ui16),
		4, 6,
		vertData, indexData
	);
	OPmeshPackerBuild();

	plane = (OPmesh*)OPcmanGet("BiPlane.opm");

	OPvec3 pos = {0, 5, 15.0f};
	OPvec3 look = {0, 0, 0};
	OPvec3 up = {0, 1, 0};

	camera = OPcamProj(
		pos,
		look,
		up,
		0.1f,
		100.0f,
		90,
		(width / (OPfloat)height)
	);

	//OPshaderAttribute attribs[] = {
	//	{"aVertexPosition",GL_FLOAT,3},
	//	{"aColor",GL_FLOAT,3}
	//};

	OPshaderAttribute attribs[] = {
		{"aPosition",GL_FLOAT,3},
		{"aNormal",GL_FLOAT,3},
		{"aTangent",GL_FLOAT,3},
		{"aUV",GL_FLOAT,2}
	};

	tri = OPrenderCreateEffect(
		*(OPshader*)OPcmanGet("TexturedSpecular.vert"),
		*(OPshader*)OPcmanGet("TexturedSpecular.frag"),
		attribs,
		4
	);

	OPLog("GameManager:Constructor Finished");
}

bool GameManager::Update( OPtimer* coreTimer )
{
	rotateAmnt += 0.005f * coreTimer->Elapsed;
	return true;
}


OPfloat t = 0;
void GameManager::Draw(){
	OPrenderClear(0.3f, 0.3f, 0.3f);
	OPmat4 world, view, proj;
	world = OPmat4();
	view = OPmat4();
	proj = OPmat4();
	t+=0.01f;

	OPmat4buildRotY(&world, t);
	OPcamGetView(camera, &view);
	OPcamGetProj(camera, &proj);

	//OPmeshPackerBind(&packer);
	OPrenderBindMesh(plane);
	OPrenderBindEffect(&tri);

	OPrenderParamMat4v("uWorld", 1, &world);
	OPrenderParamMat4v("uProj", 1, &proj);
	OPrenderParamMat4v("uView", 1, &view);

	OPrenderMesh();
}
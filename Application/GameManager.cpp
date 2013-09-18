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
OPeffect tri;
OPcam camera;

GameManager::GameManager(int width, int height) 
{
	rotateAmnt = 0;
	OPrenderInit(width, height);

	OPcmanLoad("Triangle3D.vert");
	OPcmanLoad("Triangle.frag");

	packer = OPmeshPackerCreate();

	OPmeshPackerBind(&packer);
	quad = OPrenderCreateMeshPacked(
		sizeof(OPfloat) * 6, sizeof(ui16),
		4, 6,
		vertData, indexData
	);
	OPmeshPackerBuild();

	OPvec3 pos = {0, 0.5f, 1.0f};
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

	OPshaderAttribute attribs[] = {
		{"aVertexPosition",GL_FLOAT,3},
		{"aColor",GL_FLOAT,3}
	};

	tri = OPrenderCreateEffect(
		*(OPshader*)OPcmanGet("Triangle3D.vert"),
		*(OPshader*)OPcmanGet("Triangle.frag"),
		attribs,
		2
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
	t+=0.01f;

	OPmat4buildRotY(world, t);
	OPcamGetView(camera, view);
	OPcamGetProj(camera, proj);

	OPmeshPackerBind(&packer);
	OPrenderBindEffect(&tri);

	OPrenderParamMat4v("uWorld", 1, &world);
	OPrenderParamMat4v("uProj", 1, &proj);
	OPrenderParamMat4v("uView", 1, &view);

	OPrenderMeshPacked(&quad);
}
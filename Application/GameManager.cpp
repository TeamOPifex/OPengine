#include "GameManager.h"
#include "./Human/include/Utilities/OBJLoader.h"
#include "./Data/include/OPfile.h"
#include "./Data/include/OPcontentManager.h"
#include "./Human/include/Resources/Texture/ImageDDS.h"
#include "./Human/include/Resources/Texture/ImagePNG.h"
#include "./Human/include/Utilities/Errors.h"
#include "./Human/include/Utilities/OPMLoader.h"
#include "./Human/include/Rendering/Buffer.h"
#include "./Human/include/Rendering/OPeffect.h"
#include "./Core/include/Log.h"

OPfloat vertData[] = {
	   0,  0.5, 0,
	   1,  0,   0,
	-0.5, -0.5, 0,
	   0,  1,   0,
	 0.5, -0.5, 0,
	   0,  0,   1
};
OPrenderBuffer verts;
OPeffect tri;

GameManager::GameManager(int width, int height) 
{
	rotateAmnt = 0;
	OPrenderInit(width, height);

	OPcmanLoad("Triangle3D.vert");
	OPcmanLoad("Triangle.frag");

	verts = OPrenderGenBuffer(OPvertexBuffer);
	OPrenderSetBufferData(&verts, sizeof(OPfloat) * 6, 3, vertData);

	OPshaderAttribute attribs[] = {
		{"aVertexPosition",GL_FLOAT,3,(void*)0},
		{"aColor",GL_FLOAT,3,(void*)12}
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

void GameManager::Draw(){
	OPrenderClear(0.3f, 0.3f, 0.3f);

	OPrenderBindBuffer(&verts);
	OPrenderBindEffect(&tri);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(OPfloat), (void*)0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(OPfloat), (void*)(sizeof(OPfloat) * 3));

	OPrender();
}
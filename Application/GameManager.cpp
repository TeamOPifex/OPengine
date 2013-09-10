#include "GameManager.h"
#include "./Human/include/Utilities/OBJLoader.h"
#include "./Data/include/OPfile.h"
#include "./Data/include/OPcontentManager.h"
#include "./Human/include/Resources/Texture/ImageDDS.h"
#include "./Human/include/Resources/Texture/ImagePNG.h"
#include "./Human/include/Utilities/Errors.h"
#include "./Human/include/Utilities/OPMLoader.h"
#include "./Core/include/Log.h"

GameManager::GameManager(int width, int height) 
{
	
	rotateAmnt = 0;

	OPLog("GameManager:Constructor Finished");
}

bool GameManager::Update( OPtimer* coreTimer )
{
	rotateAmnt += 0.005f * coreTimer->Elapsed;
	return true;
}

void GameManager::Draw(){

}
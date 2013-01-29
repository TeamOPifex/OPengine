#include "GameManager.h"
#include "./Human/Rendering/RenderSystem.h"

GameManager::GameManager(int width, int height) 
{	
	RenderSystem::Initialize(width, height);
}

bool GameManager::Update( OPtimer* coreTimer )
{
	return true;
}

void GameManager::Draw(){
	
}
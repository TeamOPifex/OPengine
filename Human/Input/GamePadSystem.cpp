#include "GamePadSystem.h"

GamePadState GamePadSystem::_gamePadStates[CONTROLLERS];

GamePadState* GamePadSystem::Controller(GamePadIndex index){
	return &_gamePadStates[index];
}

void GamePadSystem::Update(){
		for(int i = 0; i < CONTROLLERS; i++)
			_gamePadStates[i].update();
}
#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "./Core/include/Timer.h"

class GameManager
{
public:
	GameManager(int width, int height);
	bool Update( OPtimer* coreTimer );
	void Draw();
};

#endif
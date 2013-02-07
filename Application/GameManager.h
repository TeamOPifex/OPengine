#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "./Core/include/Timer.h"
#include "./Human/Resources/Model/Model.h"
#include "./Human/Rendering/Camera.h"
#include "./Human/Resources/Material/TexturedMaterial.h"

class GameManager
{
public:
	GameManager(int width, int height);
	bool Update( OPtimer* coreTimer );
	void Draw();
private:
	ModelPtr _model;
	SpecularTexturedMaterial* _material;
	Texture2D* _colorTexture;
	Texture2D* _normalTexture;
	Texture2D* _specularTexture;
};

#endif
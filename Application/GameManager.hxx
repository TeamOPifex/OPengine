#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "./Core/include/Timer.h"
#include "./Human/include/Resources/Model/Model.h"
#include "./Human/include/Rendering/Renderer.h"
#include "./Human/include/Rendering/Camera.h"
#include "./Human/include/Resources/Material/SpecularTexturedMaterial.h"
#include "./Human/include/Resources/Material/TexturedScreenMaterial.h"

class GameManager
{
public:
	GameManager(int width, int height);
	bool Update( OPtimer* coreTimer );
	void Draw();
private:
	//MeshPacker* meshPacker;
	//PackedModelPtr _model;
	//SpecularTexturedMaterial* _material;
	//Texture2D* _colorTexture;
	//Texture2D* _normalTexture;
	//Texture2D* _specularTexture;

	f32 rotateAmnt;
};

#endif
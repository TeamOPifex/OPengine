#include "GameManager.h"
#include "./Human/include/Rendering/RenderSystem.h"
#include "./Human/include/Utilities/OBJLoader.h"
#include "./Data/include/OPfile.h"
#include "./Human/include/Resources/Texture/ImageDDS.h"
#include "./Human/include/Resources/Texture/ImagePNG.h"
#include "./Human/include/Utilities/Errors.h"
#include "./Human/include/Utilities/OPMLoader.h"
#include "./Core/include/Log.h"

GameManager::GameManager(int width, int height) 
{
	RenderSystem::Initialize(width, height);

	_colorTexture = ImagePNG::TextureFromFile("Textures/test.png");	
	FileInformation t_file = OPreadFileInformation("Models/BiPlane.opm");
	MeshPtr mesh = LoadOPM(t_file.file);

	_normalTexture = ImageDDS::TextureFromFile("Textures/modelNormal.dds");
	_specularTexture = ImageDDS::TextureFromFile("Textures/modelSpecular.dds");

	OPLog("GameManager:Files Loaded");
	
	_material = new SpecularTexturedMaterial();
	OPLog("GameManager:Specular");
	RenderSystem::UseMaterial(_material);
	OPLog("GameManager:Material Used");
	_material->SetTextures(_colorTexture, _normalTexture, _specularTexture);
	OPLog("GameManager:Textures Set");

	_model = new Model(mesh, _material);
	OPLog("GameManager:Model Created");
	_model->WorldMatrix->SetIdentity()->Scale(3.0f);
	OPLog("GameManager:Identity Set");

	rotateAmnt = 0;

	OPLog("GameManager:Constructor Finished");
}

bool GameManager::Update( OPtimer* coreTimer )
{
	rotateAmnt += 0.0005f;
	return true;
}

void GameManager::Draw(){
	RenderSystem::ClearColor(1,0,0);
	RenderSystem::UseMaterial(_material);

	_material->View->SetMatrix(Camera::GameCamera.GetView());
	_material->Projection->SetMatrix(Camera::GameCamera.GetProj());

	_model->WorldMatrix->RotateY(0.001f)->RotateX(0.003f);
	
	_material->World->SetMatrix(_model->WorldMatrix);

	RenderSystem::RenderModel(_model);
}
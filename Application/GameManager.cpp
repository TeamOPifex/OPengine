#include "GameManager.h"
#include "./Human/Rendering/RenderSystem.h"
#include "./Human/Utilities/OBJLoader.h"
#include "./Data/include/OPfile.h"
#include "./Human/Resources/Texture/ImageDDS.h"
#include "./Human/Resources/Texture/ImagePNG.h"
#include "./Human/Utilities/Errors.h"
#include "./Human/Utilities/OPMLoader.h"

GameManager::GameManager(int width, int height) 
{
	RenderSystem::Initialize(width, height);
	
	FileInformation t_file = OPreadFileInformation("C:\\_Repos\\BiPlane.opm");
	MeshPtr mesh = LoadOPM(t_file.file);
	
	//_colorTexture = ImageDDS::TextureFromFile("Textures/modelColor.dds");
	_colorTexture = ImagePNG::TextureFromFile("Textures/test.png");
	_normalTexture = ImageDDS::TextureFromFile("Textures/modelNormal.dds");
	_specularTexture = ImageDDS::TextureFromFile("Textures/modelSpecular.dds");
	
	_material = new SpecularTexturedMaterial();
	RenderSystem::UseMaterial(_material);
	_material->SetTextures(_colorTexture, _normalTexture, _specularTexture);

	_model = new Model(mesh, _material);

	rotateAmnt = 0;
}

bool GameManager::Update( OPtimer* coreTimer )
{
	rotateAmnt += 0.0005f;
	return true;
}

void GameManager::Draw(){
	RenderSystem::ClearColor(1,0,0);
	RenderSystem::UseMaterial(_material);

	_material->View->SetMatrix(&Camera::GameCamera.GetView());
	_material->Projection->SetMatrix(&Camera::GameCamera.GetProj());
	
	(*_model->WorldMatrix) = Matrix4::RotateY(rotateAmnt).Multiply(Matrix4::Scale(5.0f));
	_material->World->SetMatrix(_model->WorldMatrix);

	RenderSystem::RenderModel(_model);
}
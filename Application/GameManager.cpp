#include "GameManager.h"
#include "./Human/Rendering/RenderSystem.h"
#include "./Human/Utilities/OBJLoader.h"
#include "./Data/include/OPfile.h"
#include "./Human/Resources/Texture/ImageDDS.h"
#include "./Human/Utilities/Errors.h"
#include "./Human/Utilities/OPMLoader.h"

GameManager::GameManager(int width, int height) 
{
	RenderSystem::Initialize(width, height);
	FileInformation info = OPreadFileInformation("Models/model.obj");
	
	FileInformation t_file = OPreadFileInformation("C:\\_Repos\\BiPlane.opm");
	MeshPtr mesh = LoadOPM(t_file.file, t_file.start, t_file.length);

	//mesh = LoadOBJ(info.file, info.start, info.length);
	_material = new SpecularTexturedMaterial();
	
	ImagePtr colorDDS = ImageDDS::FromFile("Textures/modelColor.dds");
	ImagePtr normalDDS = ImageDDS::FromFile("Textures/modelNormal.dds");
	ImagePtr specularDDS = ImageDDS::FromFile("Textures/modelSpecular.dds");
	_colorTexture = new Texture2D(colorDDS);
	_normalTexture = new Texture2D(normalDDS);
	_specularTexture = new Texture2D(specularDDS);
	delete colorDDS;
	delete normalDDS;
	delete specularDDS;
	
	CheckError("GameManager::Error 0");
	
	RenderSystem::UseMaterial(_material);
	_material->ColorTexture->SetTexture(_colorTexture, 0);
	_material->NormalTexture->SetTexture(_normalTexture, 1);
	_material->SpecularTexture->SetTexture(_specularTexture, 2);
	
	CheckError("GameManager::Error 0");

	_model = new Model(mesh, _material);
	_material->World->SetMatrix(_model->WorldMatrix);
	
	Matrix4 v = Camera::GameCamera.GetView();
	Matrix4 p = Camera::GameCamera.GetProj();

	CheckError("GameManager::Error 0");
	
	_material->View->SetMatrix(&v);
	_material->Projection->SetMatrix(&p);

	(*_model->WorldMatrix) = Matrix4::Scale(5.0f);
	rotateAmnt = 0;
}

bool GameManager::Update( OPtimer* coreTimer )
{
	return true;
}

void GameManager::Draw(){
	RenderSystem::ClearColor(1,0,0);
	RenderSystem::UseMaterial(_material);

	Matrix4 v = Camera::GameCamera.GetView();
	Matrix4 p = Camera::GameCamera.GetProj();	
	_material->View->SetMatrix(&v);
	_material->Projection->SetMatrix(&p);
	
	_material->ColorTexture->SetTexture(_colorTexture, 0);
	_material->NormalTexture->SetTexture(_normalTexture, 1);
	_material->SpecularTexture->SetTexture(_specularTexture, 2);
	
	rotateAmnt += 0.0005f;
	(*_model->WorldMatrix) = Matrix4::RotateY(rotateAmnt).Multiply(Matrix4::Scale(5.0f));
	_material->World->SetMatrix(_model->WorldMatrix);

	RenderSystem::RenderModel(_model);
}
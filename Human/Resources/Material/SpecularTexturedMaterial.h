#pragma once 
#include "Material.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamWorld.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamView.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamProjection.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamColorTexture.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamNormalTexture.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamSpecularTexture.h"
#include "./Human/Resources/Material/Shader/Attributes/ShaderAttributePosition.h"
#include "./Human/Resources/Material/Shader/Attributes/ShaderAttributeNormal.h"
#include "./Human/Resources/Material/Shader/Attributes/ShaderAttributeUV.h"
#include "./Human/Resources/Material/Shader/Attributes/ShaderAttributeTangent.h"
#include "./Human/Rendering/Camera.h"

class SpecularTexturedMaterial : public Material{
public:
	SpecularTexturedMaterial() : Material(Shader::FromFile(Vertex, "Shaders/TexturedSpecular.vert"), Shader::FromFile(Fragment, "Shaders/TexturedSpecular.frag"))
	{
		World = new ShaderParamWorld(this);
		View = new ShaderParamView(this);
		Projection = new ShaderParamProjection(this);
		ColorTexture = new ShaderParamColorTexture(this);
		NormalTexture = new ShaderParamNormalTexture(this);
		SpecularTexture = new ShaderParamSpecularTexture(this);
		
		_Position = new ShaderAttributePosition(this);
		_Normal = new ShaderAttributeNormal(this);
		_UV = new ShaderAttributeUV(this);
		_Tangent = new ShaderAttributeTangent(this);
	}

	~SpecularTexturedMaterial(){
		delete World;
		delete View;
		delete Projection;
		delete ColorTexture;
		delete NormalTexture;
		delete SpecularTexture;
		delete _Position;
		delete _Normal;
		delete _UV;
		delete _Tangent;
	}

	void EnableAttributes(){
		_Position->Enable();
		_Normal->Enable();
		_UV->Enable();
		_Tangent->Enable();
	}

	void DisableAttributes(){
		_Position->Disable();
		_Normal->Disable();
		_UV->Disable();
		_Tangent->Disable();
	}

	void SetMeshData(Mesh* mesh){
		Material::SetData(_Position->Handle(), 3, false, mesh->Stride, (void*)0);
		Material::SetData(_Normal->Handle(), 3, false, mesh->Stride, (void*)12);
		Material::SetData(_UV->Handle(), 2, false, mesh->Stride, (void*)24);
		Material::SetData(_Tangent->Handle(), 3, false, mesh->Stride, (void*)32);		
	}
	
	void SetColorTexture(Texture2D* tex) {
		ColorTexture->SetTexture(tex, 0);
	}

	void SetNormalTexture(Texture2D* tex) {
		NormalTexture->SetTexture(tex, 1);
	}

	void SetSpecularTexture(Texture2D* tex) {
		SpecularTexture->SetTexture(tex, 2);
	}

	void SetTextures(Texture2D* color, Texture2D* normal, Texture2D* specular) {
		ColorTexture->SetTexture(color, 0);
		NormalTexture->SetTexture(normal, 1);
		SpecularTexture->SetTexture(specular, 2);
	}

	void SetViewProjection() {
		View->SetMatrix(Camera::GameCamera.GetView());
		Projection->SetMatrix(Camera::GameCamera.GetProj());
	}
	
	ShaderParamWorld* World;
	ShaderParamView* View;
	ShaderParamProjection* Projection;
	ShaderParamColorTexture* ColorTexture;
	ShaderParamNormalTexture* NormalTexture;
	ShaderParamSpecularTexture* SpecularTexture;

private:
	ShaderAttributePosition* _Position;
	ShaderAttributeNormal* _Normal;
	ShaderAttributeUV* _UV;
	ShaderAttributeTangent* _Tangent;
};



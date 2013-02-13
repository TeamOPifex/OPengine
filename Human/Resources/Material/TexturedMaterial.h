#pragma once 
#include "Material.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamWorld.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamView.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamProjection.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamColorTexture.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamNormalTexture.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamSpecularTexture.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamAlpha.h"
#include "./Human/Resources/Material/Shader/Attributes/ShaderAttributePosition.h"
#include "./Human/Resources/Material/Shader/Attributes/ShaderAttributeNormal.h"
#include "./Human/Resources/Material/Shader/Attributes/ShaderAttributeUV.h"
#include "./Human/Resources/Material/Shader/Attributes/ShaderAttributeTangent.h"
#include "./Human/Resources/Model/Mesh.h"
#include "./Core/include/DynamicMemory.h"

class TexturedMaterial : public Material{
public:
	TexturedMaterial() : Material(Shader::FromFile(Vertex, ""), Shader::FromFile(Fragment, ""))
	{
		World = new ShaderParamWorld(this);
		View = new ShaderParamView(this);
		Projection = new ShaderParamProjection(this);
		ColorTexture = new ShaderParamColorTexture(this);
	}

	~TexturedMaterial(){
		delete World;
		delete View;
		delete Projection;
		delete ColorTexture;
	}

	ShaderParamWorld* World;
	ShaderParamView* View;
	ShaderParamProjection* Projection;
	ShaderParamColorTexture* ColorTexture;
};

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


class TexturedScreenMaterial : public Material{
public:
	TexturedScreenMaterial() : Material(Shader::FromFile(Vertex, "Shaders/TexturedScreen.vert"), Shader::FromFile(Fragment, "Shaders/Textured.frag"))
	{
		World = new ShaderParamWorld(this);
		ColorTexture = new ShaderParamColorTexture(this);
		Alpha = new ShaderParamAlpha(this);
		
		_Position = new ShaderAttributePosition(this);
		_UV = new ShaderAttributeUV(this);
	}

	~TexturedScreenMaterial(){
		delete World;
		delete ColorTexture;
		delete Alpha;
		delete _Position;
		delete _UV;
	}

	void EnableAttributes(){
		_Position->Enable();
		_UV->Enable();
	}

	void DisableAttributes(){
		_Position->Disable();
		_UV->Disable();
	}

	void SetMeshData(Mesh* mesh){
		Material::SetData(_Position->Handle(), 3, false, mesh->Stride, (void*)0);
		Material::SetData(_UV->Handle(), 2, false, mesh->Stride, (void*)12);
	}
	
	ShaderParamWorld* World;
	ShaderParamColorTexture* ColorTexture;
	ShaderParamAlpha* Alpha;
private:
	ShaderAttributePosition* _Position;
	ShaderAttributeUV* _UV;
};

class BlueScreenMaterial : public Material{
public:
	BlueScreenMaterial() : Material(Shader::FromFile(Vertex, "Shaders/Screen.vert"), Shader::FromFile(Fragment, "Shaders/Blue.frag"))
	{
		World = new ShaderParamWorld(this);		
		_Position = new ShaderAttributePosition(this);
	}

	~BlueScreenMaterial(){
		delete World;
		delete _Position;
	}

	void EnableAttributes(){
		_Position->Enable();
	}

	void DisableAttributes(){
		_Position->Disable();
	}

	void SetMeshData(Mesh* mesh){
		Material::SetData(_Position->Handle(), 3, false, mesh->Stride, (void*)0);
	}
	
	ShaderParamWorld* World;
private:
	ShaderAttributePosition* _Position;
};

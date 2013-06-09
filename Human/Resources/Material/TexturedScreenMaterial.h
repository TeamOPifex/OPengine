#pragma once 
#include "Material.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamWorld.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamColorTexture.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamAlpha.h"
#include "./Human/Resources/Material/Shader/Attributes/ShaderAttributePosition.h"
#include "./Human/Resources/Material/Shader/Attributes/ShaderAttributeUV.h"

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
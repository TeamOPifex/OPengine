#pragma once 
#include "Material.h"
#include "./Human/include/Resources/Material/Shader/Params/ShaderParamWorld.h"
#include "./Human/include/Resources/Material/Shader/Attributes/ShaderAttributePosition.h"

class BlueScreenMaterial : public Material{
public:
	BlueScreenMaterial() : Material(Shader::FromFile(Vertex, "Shaders/Screen.vert"), Shader::FromFile(Fragment, "Shaders/Blue.frag"))
	{
		World = new ShaderParamWorld(this);		
		_Position = new ShaderAttributePosition(this);
	}

	~BlueScreenMaterial(){
	}

	void Destroy() {
		delete World;
		delete _Position;
		Material::Destroy();
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

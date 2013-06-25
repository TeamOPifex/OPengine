#pragma once 
#include "Material.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamWorld.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamColorTexture.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamAlpha.h"
#include "./Human/Resources/Material/Shader/Attributes/ShaderAttributePosition.h"
#include "./Human/Resources/Material/Shader/Attributes/ShaderAttributeUV.h"

class TexturedScreenMaterial : public Material{
public:
	TexturedScreenMaterial();
	~TexturedScreenMaterial();

	void EnableAttributes();
	void DisableAttributes();
	void SetMeshData(Mesh* mesh);
	
	ShaderParamWorld* World;
	ShaderParamColorTexture* ColorTexture;
	ShaderParamAlpha* Alpha;
private:
	ShaderAttributePosition* _Position;
	ShaderAttributeUV* _UV;
};
#pragma once 
#include "Material.h"
#include "./Human/include/Resources/Material/Shader/Params/ShaderParamWorld.h"
#include "./Human/include/Resources/Material/Shader/Params/ShaderParamColorTexture.h"
#include "./Human/include/Resources/Material/Shader/Params/ShaderParamAlpha.h"
#include "./Human/include/Resources/Material/Shader/Attributes/ShaderAttributePosition.h"
#include "./Human/include/Resources/Material/Shader/Attributes/ShaderAttributeUV.h"

class TexturedScreenMaterial : public Material{
public:
	TexturedScreenMaterial();
	~TexturedScreenMaterial();
	void Destroy();

	void EnableAttributes();
	void DisableAttributes();
	void SetMeshData(BaseMeshPtr mesh);
	
	ShaderParamWorld* World;
	ShaderParamColorTexture* ColorTexture;
	ShaderParamAlpha* Alpha;
private:
	ShaderAttributePosition* _Position;
	ShaderAttributeUV* _UV;
};
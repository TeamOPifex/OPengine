#pragma once 
#include "Material.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamWorld.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamView.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamProjection.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamColorTexture.h"

class TexturedMaterial : public Material{
public:
	TexturedMaterial();

	~TexturedMaterial();

	ShaderParamWorld* World;
	ShaderParamView* View;
	ShaderParamProjection* Projection;
	ShaderParamColorTexture* ColorTexture;
};

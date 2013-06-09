#pragma once 
#include "Material.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamWorld.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamView.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamProjection.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamColorTexture.h"

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

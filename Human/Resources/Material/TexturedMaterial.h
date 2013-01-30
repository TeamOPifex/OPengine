#pragma once 
#include "Material.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamWorld.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamView.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamProjection.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamColorTexture.h"
#include "./Core/include/DynamicMemory.h"

class TexturedMaterial : Material{
public:
	TexturedMaterial() : Material(Shader::FromFile(Vertex, ""), Shader::FromFile(Fragment, ""))
	{
		World = new ShaderParamWorld(this);
		View = new ShaderParamView(this);
		Projection = new ShaderParamProjection(this);
		Texture = new ShaderParamColorTexture(this);
	}

	~TexturedMaterial(){
		delete World;
		delete View;
		delete Projection;
		delete Texture;
	}

	ShaderParamWorld* World;
	ShaderParamView* View;
	ShaderParamProjection* Projection;
	ShaderParamColorTexture* Texture;
};

#include "./Human/include/Resources/Material/TexturedMaterial.h"

TexturedMaterial::TexturedMaterial(bool init) : Material()
{
	if(init) Init();
}

void TexturedMaterial::Init(){
	World = new ShaderParamWorld(this);
	View = new ShaderParamView(this);
	Projection = new ShaderParamProjection(this);
	ColorTexture = new ShaderParamColorTexture(this);
	Material::Init(Shader::FromFile(Vertex, ""), Shader::FromFile(Fragment, ""));
}

TexturedMaterial::~TexturedMaterial(){
	Destroy();
}

void TexturedMaterial::Destroy(){
	delete World;
	delete View;
	delete Projection;
	delete ColorTexture;
	Material::Destroy();
}
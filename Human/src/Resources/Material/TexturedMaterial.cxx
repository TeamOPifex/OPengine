#include "./Human/include/Resources/Material/TexturedMaterial.h"

TexturedMaterial::TexturedMaterial() : Material(Shader::FromFile(Vertex, ""), Shader::FromFile(Fragment, ""))
{
	World = new ShaderParamWorld(this);
	View = new ShaderParamView(this);
	Projection = new ShaderParamProjection(this);
	ColorTexture = new ShaderParamColorTexture(this);
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
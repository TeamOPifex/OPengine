#include "./Human/include/Resources/Material/TexturedScreenMaterial.h"

TexturedScreenMaterial::TexturedScreenMaterial() : Material(Shader::FromFile(Vertex, "Shaders/TexturedScreen.vert"), Shader::FromFile(Fragment, "Shaders/Textured.frag"))
{
	World = new ShaderParamWorld(this);
	ColorTexture = new ShaderParamColorTexture(this);
	Alpha = new ShaderParamAlpha(this);
		
	_Position = new ShaderAttributePosition(this);
	_UV = new ShaderAttributeUV(this);
}

TexturedScreenMaterial::~TexturedScreenMaterial(){
	delete World;
	delete ColorTexture;
	delete Alpha;
	delete _Position;
	delete _UV;
}

void TexturedScreenMaterial::EnableAttributes(){
	_Position->Enable();
	_UV->Enable();
}

void TexturedScreenMaterial::DisableAttributes(){
	_Position->Disable();
	_UV->Disable();
}

void TexturedScreenMaterial::SetMeshData(BaseMeshPtr mesh){
	Material::SetData(_Position->Handle(), 3, false, mesh->Stride, (void*)0);
	Material::SetData(_UV->Handle(), 2, false, mesh->Stride, (void*)12);
}
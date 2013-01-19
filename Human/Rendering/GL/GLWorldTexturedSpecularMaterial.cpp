#include "GLWorldTexturedSpecularMaterial.h"
#include "./Core/include/Log.h"

GLWorldTexturedSpecularMaterial::GLWorldTexturedSpecularMaterial(){
}

GLWorldTexturedSpecularMaterial::GLWorldTexturedSpecularMaterial(ShaderPtr vertex, ShaderPtr fragment) : GLWorldMaterial(vertex, fragment){
	_normalLoc = attribute_location("vNormal");
	_tangentLoc = attribute_location("vTangent");
	_uvLoc = attribute_location("TexCoordIn");
	_textureLoc = uniform_location("Texture");
	_textureNormalLoc = uniform_location("NormalTexture");
	_textureSpecularLoc = uniform_location("SpecularTexture");
}

ui32 GLWorldTexturedSpecularMaterial::GetTextureLocation(){
	return _textureLoc;
}

ui32 GLWorldTexturedSpecularMaterial::GetTextureNormalLocation(){
	return _textureNormalLoc;
}

ui32 GLWorldTexturedSpecularMaterial::GetTextureSpecularLocation(){
	return _textureSpecularLoc;
}

ui32 GLWorldTexturedSpecularMaterial::SetTexture(GLTexture* texture, ui32 slot){
	texture->bind(_textureLoc, slot);
}

ui32 GLWorldTexturedSpecularMaterial::SetTextureNormal(GLTexture* texture, ui32 slot){
	texture->bind(_textureNormalLoc, slot);
}

ui32 GLWorldTexturedSpecularMaterial::SetTextureSpecular(GLTexture* texture, ui32 slot){
	texture->bind(_textureSpecularLoc, slot);
}

void GLWorldTexturedSpecularMaterial::EnableAttributes(){
	enable_attrib(_positionLoc);
	enable_attrib(_normalLoc);
	enable_attrib(_tangentLoc);
	enable_attrib(_uvLoc);
	enable_attrib(_textureLoc);
	enable_attrib(_textureNormalLoc);
	enable_attrib(_textureSpecularLoc);
}

void GLWorldTexturedSpecularMaterial::DisableAttributes(){
	disable_attrib(_positionLoc);
	disable_attrib(_normalLoc);
	disable_attrib(_tangentLoc);
	disable_attrib(_uvLoc);
	disable_attrib(_textureLoc);
	disable_attrib(_textureNormalLoc);
	disable_attrib(_textureSpecularLoc);
}

void GLWorldTexturedSpecularMaterial::SetUVData(ui32 stride, void* data){
	set_data(_uvLoc, 2, false, stride, data);
}

void GLWorldTexturedSpecularMaterial::SetNormalData(ui32 stride, void* data){
	set_data(_normalLoc, 3, false, stride, data);
}

void GLWorldTexturedSpecularMaterial::SetTangentData(ui32 stride, void* data){
	set_data(_tangentLoc, 3, false, stride, data);
}

void GLWorldTexturedSpecularMaterial::SetData(Mesh* mesh){
	SetPositionData(mesh->Stride, (void*)0);
	SetUVData(mesh->Stride, (void*)12);
	SetNormalData(mesh->Stride, (void*)20);
	SetTangentData(mesh->Stride, (void*)32);
	OPLog("GLWorldTexturedSpecularMaterial::SetData");
}
#include "GLTexturedMaterial.h"
#include "./Core/include/Log.h"

GLTexturedMaterial::GLTexturedMaterial(){
}

GLTexturedMaterial::GLTexturedMaterial(ShaderPtr vertex, ShaderPtr fragment) : GLWorldMaterial(vertex, fragment){
	_positionLoc = attribute_location("vPosition");
	_uvLoc = attribute_location("TexCoordIn");
	_textureLoc = uniform_location("Texture");
	_worldMatrix = uniform_location("Model");
}

ui32 GLTexturedMaterial::GetTextureLocation(){
	return _textureLoc;
}

void GLTexturedMaterial::SetTexture(GLTexture* texture, ui32 slot){
	texture->bind(_textureLoc, slot);
}

void GLTexturedMaterial::EnableAttributes(){
	enable_attrib(_positionLoc);
	enable_attrib(_uvLoc);
	enable_attrib(_textureLoc);
}

void GLTexturedMaterial::DisableAttributes(){
	disable_attrib(_positionLoc);
	disable_attrib(_uvLoc);
	disable_attrib(_textureLoc);
}

void GLTexturedMaterial::SetUVData(ui32 stride, void* data){
	set_data(_uvLoc, 2, false, stride, data);
}

void GLTexturedMaterial::SetData(Mesh* mesh){
	SetPositionData(mesh->Stride, (void*)0);
	SetUVData(mesh->Stride, (void*)12);
	OPLog("GLWorldTexturedMaterial::SetData");
}

void GLTexturedMaterial::SetPositionData(ui32 stride, void* data)
{
	set_data(_positionLoc, 3, false, stride, data);
}
#include "GLWorldTexturedMaterial.h"
#include "./Core/include/Log.h"

GLWorldTexturedMaterial::GLWorldTexturedMaterial(){
}

GLWorldTexturedMaterial::GLWorldTexturedMaterial(ShaderPtr vertex, ShaderPtr fragment) : GLWorldMaterial(vertex, fragment){
	_uvLoc = attribute_location("TexCoordIn");
	_textureLoc = uniform_location("Texture");
}

ui32 GLWorldTexturedMaterial::GetTextureLocation(){
	return _textureLoc;
}

ui32 GLWorldTexturedMaterial::SetTexture(GLTexture* texture, ui32 slot){
	texture->bind(_textureLoc, slot);
}

void GLWorldTexturedMaterial::EnableAttributes(){
	enable_attrib(_positionLoc);
	enable_attrib(_uvLoc);
	enable_attrib(_textureLoc);
}

void GLWorldTexturedMaterial::DisableAttributes(){
	disable_attrib(_positionLoc);
	disable_attrib(_uvLoc);
	disable_attrib(_textureLoc);
}

void GLWorldTexturedMaterial::SetUVData(ui32 stride, void* data){
	set_data(_uvLoc, 2, false, stride, data);
}

void GLWorldTexturedMaterial::SetData(Mesh* mesh){
	SetPositionData(mesh->Stride, (void*)0);
	SetUVData(mesh->Stride, (void*)12);
	OPLog("GLWorldTexturedMaterial::SetData");
}
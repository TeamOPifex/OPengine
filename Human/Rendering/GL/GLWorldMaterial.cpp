#include "GLWorldMaterial.h"
#include "./Core/include/Log.h"

GLWorldMaterial::GLWorldMaterial(){
}

GLWorldMaterial::GLWorldMaterial(ShaderPtr vertex, ShaderPtr fragment) : GLMaterial(vertex, fragment) {
		_worldMatrix = uniform_location("Model");
		_positionLoc = attribute_location("vPosition");
}

void GLWorldMaterial::SetWorldMatrix(f32* world) 
{ 
	set_matrix(_worldMatrix, world); 
}

void GLWorldMaterial::SetPositionData(ui32 stride, void* data)
{
	set_data(_positionLoc, 3, false, stride, data);
}

void GLWorldMaterial::SetData(Mesh* mesh){
	SetPositionData(mesh->Stride, (void*)0);
	OPLog("GLWorldMaterial::SetData");
}
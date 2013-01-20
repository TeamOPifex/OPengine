#include "GLWorldProjMaterial.h"
#include "./Core/include/Log.h"

GLWorldProjMaterial::GLWorldProjMaterial(){
}

GLWorldProjMaterial::GLWorldProjMaterial(ShaderPtr vertex, ShaderPtr fragment) : GLWorldMaterial(vertex, fragment) {
		_viewProjectionMatrix = uniform_location("ViewProjection");
}

void GLWorldProjMaterial::SetViewProjectionMatrix(f32* world) 
{ 
	set_matrix(_viewProjectionMatrix, world); 
}

void GLWorldProjMaterial::SetPositionData(ui32 stride, void* data)
{
	set_data(_positionLoc, 3, false, stride, data);
}

void GLWorldProjMaterial::SetData(Mesh* mesh){
	SetPositionData(mesh->Stride, (void*)0);
	OPLog("GLWorldMaterial::SetData");
}
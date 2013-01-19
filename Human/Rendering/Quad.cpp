#include "Quad.h"

#include "MeshVertexTextured.h"
#include "./Core/include/Log.h"
#include "./Human/Rendering/RenderSystem.h"

Quad::Quad(GLWorldTexturedSpecularMaterial* material) : Model(GenMesh(), material){
	RenderSystem::SetBuffer(1, ModelMesh->VertexBuffer->handle());
	material->SetPositionData(sizeof(MeshVertexTextured), (void*)0);
	material->SetUVData(sizeof(MeshVertexTextured), (void*)12);
	material->SetNormalData(sizeof(MeshVertexTextured), (void*)20);
	material->SetTangentData(sizeof(MeshVertexTextured), (void*)32);
	OPLog("Quad::Initialized");
}

Quad::~Quad(){
	OPfree(ModelMesh);
}

void Quad::SetPosition(Vector3* position){
	_position._x = position->_x;
	_position._y = position->_y;
	_position._z = position->_z;
	UpdateWorld();
}	

void Quad::SetRotation(Vector3* rotation){
	_rotation._x = rotation->_x;
	_rotation._y = rotation->_y;
	_rotation._z = rotation->_z;
	UpdateWorld();
}

void Quad::SetScale(Vector3* scale){
	_scale._x = scale->_x;
	_scale._y = scale->_y;
	_scale._z = scale->_z;
	UpdateWorld();
}

void Quad::SetScale(f32 scale){
	_scale._x = scale;
	_scale._y = scale;
	_scale._z = scale;
	UpdateWorld();
}

Vector3* Quad::GetPosition(){
	return &_position;
}

Vector3* Quad::GetRotation(){
	return &_rotation;
}

Vector3* Quad::GetScale(){
	return &_scale;
}

Mesh* Quad::GenMesh(){
	MeshVertexTextured* points = (MeshVertexTextured*)OPalloc(sizeof(MeshVertexTextured) * 4);
	unsigned int* indices = (unsigned int*)OPalloc(sizeof(unsigned int) * 6);
		
	points[0].vertice._x = -0.5f;
	points[0].vertice._y = 0.5f;
	points[0].vertice._z = 0.0f;
	points[0].tex._x = 0.0f;
	points[0].tex._y = 0.0f;
	points[0].normal._x = 0.0f;
	points[0].normal._y = 0.0f;
	points[0].normal._z = 0.0f;
	points[0].tangent._x = 0.0f;
	points[0].tangent._y = 0.0f;
	points[0].tangent._z = 0.0f;
		
	points[1].vertice._x = 0.5f;
	points[1].vertice._y = 0.5f;
	points[1].vertice._z = 0.0f;
	points[1].tex._x = 1.0f;
	points[1].tex._y = 0.0f;
	points[1].normal._x = 0.0f;
	points[1].normal._y = 0.0f;
	points[1].normal._z = 0.0f;
	points[1].tangent._x = 0.0f;
	points[1].tangent._y = 0.0f;
	points[1].tangent._z = 0.0f;

	points[2].vertice._x = 0.5f;
	points[2].vertice._y = -0.5f;
	points[2].vertice._z = 0.0f;
	points[2].tex._x = 1.0f;
	points[2].tex._y = 1.0f;
	points[2].normal._x = 0.0f;
	points[2].normal._y = 0.0f;
	points[2].normal._z = 0.0f;
	points[2].tangent._x = 0.0f;
	points[2].tangent._y = 0.0f;
	points[2].tangent._z = 0.0f;

	points[3].vertice._x = -0.5f;
	points[3].vertice._y = -0.5f;
	points[3].vertice._z = 0.0f;
	points[3].tex._x = 0.0f;
	points[3].tex._y = 1.0f;
	points[3].normal._x = 0.0f;
	points[3].normal._y = 0.0f;
	points[3].normal._z = 0.0f;
	points[3].tangent._x = 0.0f;
	points[3].tangent._y = 0.0f;
	points[3].tangent._z = 0.0f;
		
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
		
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	BufferPtr vertexBuffer = new GLBuffer(1, sizeof(MeshVertexTextured) * 4, points);
	BufferPtr indexBuffer = new GLBuffer(2, sizeof(int) * 6, indices);
	
	OPLog("Quad Generated");
	OPfree(points);
	OPfree(indices);

	OPLog("Returning Mesh");

	return new Mesh(vertexBuffer, indexBuffer, 6, sizeof(MeshVertexTextured));
}

void Quad::UpdateWorld(){
	Matrix4 rotate = Matrix4::RotateZ(_rotation._z);
	Matrix4 scale = Matrix4::Scale(_rotation._x, _rotation._y, _rotation._z);
	Matrix4 translate = Matrix4::Translate(_position._x, _position._y, _position._z);
	(*WorldMatrix) =  rotate * scale * translate;
}
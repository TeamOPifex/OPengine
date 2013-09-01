#include "./Human/include/Rendering/Quad.h"

#include "./Human/include/Rendering/MeshVertexTextured.h"
#include "./Core/include/Log.h"
#include "./Human/include/Rendering/RenderSystem.h"

Quad::Quad(MaterialPtr material) : Model(GenMesh(), material){

}

Quad::~Quad(){
	OPfree(ModelMesh);
}

void Quad::SetPosition(Vector3* position){
	_position.x = position->x;
	_position.y = position->y;
	_position.z = position->z;
	UpdateWorld();
}

void Quad::SetRotation(Vector3* rotation){
	_rotation.x = rotation->x;
	_rotation.y = rotation->y;
	_rotation.z = rotation->z;
	UpdateWorld();
}

void Quad::SetScale(Vector3* scale){
	_scale.x = scale->x;
	_scale.y = scale->y;
	_scale.z = scale->z;
	UpdateWorld();
}

void Quad::SetScale(f32 scale){
	_scale.x = scale;
	_scale.y = scale;
	_scale.z = scale;
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
	unsigned short* indices = (unsigned short*)OPalloc(sizeof(unsigned short) * 6);
		
	points[0].vertice.x = -0.5f;
	points[0].vertice.y = 0.5f;
	points[0].vertice.z = 0.0f;
	points[0].tex.x = 0.0f;
	points[0].tex.y = 0.0f;
		
	points[1].vertice.x = 0.5f;
	points[1].vertice.y = 0.5f;
	points[1].vertice.z = 0.0f;
	points[1].tex.x = 1.0f;
	points[1].tex.y = 0.0f;

	points[2].vertice.x = 0.5f;
	points[2].vertice.y = -0.5f;
	points[2].vertice.z = 0.0f;
	points[2].tex.x = 1.0f;
	points[2].tex.y = 1.0f;

	points[3].vertice.x = -0.5f;
	points[3].vertice.y = -0.5f;
	points[3].vertice.z = 0.0f;
	points[3].tex.x = 0.0f;
	points[3].tex.y = 1.0f;
		
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
		
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	BufferPtr vertexBuffer = new Buffer(VertexBuffer, sizeof(MeshVertexTextured) * 4, points);
	BufferPtr indexBuffer = new Buffer(IndexBuffer, sizeof(unsigned short) * 6, indices);
	
	OPfree(points);
	OPfree(indices);

	return new Mesh(vertexBuffer, indexBuffer, 6, sizeof(MeshVertexTextured));
}

void Quad::UpdateWorld(){
	Matrix4 rotate = Matrix4::BuildRotateZ(_rotation.z);
	Matrix4 scale = Matrix4::BuildScale(_rotation.x, _rotation.y, _rotation.z);
	Matrix4 translate = Matrix4::BuildTranslate(_position.x, _position.y, _position.z);
	(*WorldMatrix) =  rotate * scale * translate;
}
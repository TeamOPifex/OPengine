//#include "Sprite2D.h"
//
//#include "MeshVertexTextured.h"
//#include "./Core/include/Log.h"
//#include "./Human/Rendering/RenderSystem.h"
//#include "./Human/Rendering/GL/GLBuffer.h"
//#include ".\Core\include\DynamicMemory.h"
//
//Sprite2D::Sprite2D(GLTexturedMaterial* material) : Model(GenMesh(), material){
//	RenderSystem::SetBuffer(1, ModelMesh->VertexBuffer->handle());
//	material->SetData(ModelMesh);
//	OPLog("Quad::Initialized");
//}
//
//Sprite2D::~Sprite2D(){
//	OPfree(ModelMesh);
//}
//
//void Sprite2D::SetPosition(Vector3* position){
//	_position._x = position->_x;
//	_position._y = position->_y;
//	_position._z = position->_z;
//	UpdateWorld();
//}
//
//void Sprite2D::SetRotation(Vector3* rotation){
//	_rotation._x = rotation->_x;
//	_rotation._y = rotation->_y;
//	_rotation._z = rotation->_z;
//	UpdateWorld();
//}
//
//void Sprite2D::SetScale(Vector3* scale){
//	_scale._x = scale->_x;
//	_scale._y = scale->_y;
//	_scale._z = scale->_z;
//	UpdateWorld();
//}
//
//void Sprite2D::SetScale(f32 scale){
//	_scale._x = scale;
//	_scale._y = scale;
//	_scale._z = scale;
//	UpdateWorld();
//}
//
//Vector3* Sprite2D::GetPosition(){
//	return &_position;
//}
//
//Vector3* Sprite2D::GetRotation(){
//	return &_rotation;
//}
//
//Vector3* Sprite2D::GetScale(){
//	return &_scale;
//}
//
//Mesh* Sprite2D::GenMesh(){
//	MeshVertexTextured* points = (MeshVertexTextured*)OPalloc(sizeof(MeshVertexTextured) * 4);
//	unsigned int* indices = (unsigned int*)OPalloc(sizeof(unsigned int) * 6);
//		
//	points[0].vertice._x = -0.5f;
//	points[0].vertice._y = 0.5f;
//	points[0].vertice._z = 0.0f;
//	points[0].tex._x = 0.0f;
//	points[0].tex._y = 0.0f;
//		
//	points[1].vertice._x = 0.5f;
//	points[1].vertice._y = 0.5f;
//	points[1].vertice._z = 0.0f;
//	points[1].tex._x = 1.0f;
//	points[1].tex._y = 0.0f;
//
//	points[2].vertice._x = 0.5f;
//	points[2].vertice._y = -0.5f;
//	points[2].vertice._z = 0.0f;
//	points[2].tex._x = 1.0f;
//	points[2].tex._y = 1.0f;
//
//	points[3].vertice._x = -0.5f;
//	points[3].vertice._y = -0.5f;
//	points[3].vertice._z = 0.0f;
//	points[3].tex._x = 0.0f;
//	points[3].tex._y = 1.0f;
//		
//	indices[0] = 0;
//	indices[1] = 1;
//	indices[2] = 2;
//		
//	indices[3] = 0;
//	indices[4] = 2;
//	indices[5] = 3;
//
//	BufferPtr vertexBuffer = new GLBuffer(1, sizeof(MeshVertexTextured) * 4, points);
//	BufferPtr indexBuffer = new GLBuffer(2, sizeof(int) * 6, indices);
//	
//	OPLog("Sprite2D::Generated");
//	OPfree(points);
//	OPfree(indices);
//
//	OPLog("Returning Mesh");
//
//	return new Mesh(vertexBuffer, indexBuffer, 6, sizeof(MeshVertexTextured));
//}
//
//void Sprite2D::UpdateWorld(){
//	Matrix4 rotate = Matrix4::RotateZ(_rotation._z);
//	Matrix4 scale = Matrix4::Scale(_rotation._x, _rotation._y, _rotation._z);
//	Matrix4 translate = Matrix4::Translate(_position._x, _position._y, _position._z);
//	(*WorldMatrix) =  rotate * scale * translate;
//	OPLog("Sprite2D::World Matrix Updated");
//}
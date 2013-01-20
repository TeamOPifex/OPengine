//#pragma once
//
//#include "./Human/Rendering/Model.h"
//#include "./Human/Rendering/GL/GLTexturedMaterial.h"
//
//class Sprite2D : public Model {
//public:
//	Sprite2D(GLTexturedMaterial* material);
//	~Sprite2D();
//
//	void SetPosition(Vector3* position);
//	void SetRotation(Vector3* rotation);	
//	void SetScale(Vector3* scale);
//	void SetScale(f32 scale);
//	
//	Vector3* GetPosition();
//	Vector3* GetRotation();
//	Vector3* GetScale();
//	
//private:
//	Mesh* GenMesh();
//	void UpdateWorld();
//
//protected:
//	Vector3 _position;
//	Vector3 _rotation;
//	Vector3 _scale;
//};
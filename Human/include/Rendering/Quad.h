#pragma once

#include "./Human/include/Resources/Model/Model.h"

#include "./Core/include/DynamicMemory.h"
#include "./Math/include/Vector3.h"

class Quad : public Model {
public:
	Quad();
	Quad(MaterialPtr material);
	~Quad();

	void SetPosition(OPvec3* position);
	void SetRotation(OPvec3* rotation);	
	void SetScale(OPvec3* scale);
	void SetScale(OPfloat scale);
	
	OPvec3* GetPosition();
	OPvec3* GetRotation();
	OPvec3* GetScale();
	
private:
	Mesh* GenMesh();
	void UpdateWorld();

protected:
	OPvec3 _position;
	OPvec3 _rotation;
	OPvec3 _scale;
};
#pragma once

#include "./Human/include/Resources/Resource.h"
#include "./Shader/Shader.h"
#include "./Math/include/Vector2.h"
#include "./Math/include/Matrix4.h"
#include "./Data/include/OPhashMap.h"
#include "./Human/include/Resources/Model/Mesh.h"

class Material : public Resource {
public:
	Material();
	Material(ShaderPtr vertex, ShaderPtr fragment);
	~Material();

	ui32 GetAttribute(const char* attribute);
	ui32 GetUniform(const char* attribute);
	
	void LoadAttribute(const char* attribute);
	void LoadUniform(const char* attribute);
	
	void BindAttribute(i32 pos, char* variable);

	void EnableAttribute(ui32 attribute);
	void DisableAttribute(ui32 attribute);
	
	void SetData(ui32 attribute, ui32 size, bool normalize, ui32 stride, void* arrayData);

	void SetFloat(ui32 attribute, f32 value);
	void SetVector2(ui32 attribute, OPvec2* value);
	void SetVector3(ui32 attribute, OPvec3* value);
	void SetVector4(ui32 attribute, OPvec4* value);
	void SetMatrix(ui32 attribute, OPmat4* value);
	
	void SetFloat(const OPchar* attribute, f32 value);
	void SetVector2(const OPchar* attribute, OPvec2* value);
	void SetVector3(const OPchar* attribute, OPvec3* value);
	void SetVector4(const OPchar* attribute, OPvec4* value);
	void SetMatrix(const OPchar* attribute, OPmat4* value);

	
	virtual void Destroy();
	virtual void SetMeshData(BaseMeshPtr mesh) { }
	virtual void EnableAttributes() { }
	virtual void DisableAttributes() { }
private:
	HashMap* hashMap;
};

typedef Material* MaterialPtr;

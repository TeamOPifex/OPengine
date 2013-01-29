#pragma once

#include "./Human/Resources/Resource.h"
#include "./Shader/Shader.h"
#include "./Human/Math/Vector2.h"
#include "./Human/Math/Matrix4.h"

class Material : public Resource{
public:
	Material(ShaderPtr vertex, ShaderPtr fragment);
	ui32 GetAttribute(const char* attribute);
	ui32 GetUniform(const char* attribute);
	
	void BindAttribute(i32 pos, char* variable);

	void EnableAttribute(ui32 attribute);
	void DisableAttribute(ui32 attribute);
	
	void SetData(ui32 attribute, ui32 size, bool normalize, ui32 stride, void* arrayData);
	void SetFloat(ui32 attribute, f32 value);
	void SetVector2(ui32 attribute, Vector2* value);
	void SetVector3(ui32 attribute, Vector3* value);
	void SetVector4(ui32 attribute, Vector4* value);
	void SetMatrix(ui32 attribute, Matrix4* value);
};

typedef Material* MaterialPtr;

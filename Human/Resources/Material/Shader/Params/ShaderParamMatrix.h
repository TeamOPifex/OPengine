#pragma once
#include "ShaderParam.h"
#include "./Human/Math/Matrix4.h"
#include "./Human/Resources/Material/Material.h"

class ShaderParamMatrix : ShaderParam {
public:
	ShaderParamMatrix(Material* material, const char* variable) {
		//_handle = glGetAttribLocation(program, variable);
	}
protected:
	void SetMatrix(Matrix4* value) {
		//glUniformMatrix4fv(_handle, 1, GL_FALSE, &(*value)[0][0]);
	}
};
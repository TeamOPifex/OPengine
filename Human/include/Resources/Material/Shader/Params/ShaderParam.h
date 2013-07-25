#pragma once
#include "./Core/include/Types.h"

class ShaderParam {	
public:
	ShaderParam(MaterialPtr material) { _material = material; }
protected:
	ui32 _handle;
	MaterialPtr _material;
};
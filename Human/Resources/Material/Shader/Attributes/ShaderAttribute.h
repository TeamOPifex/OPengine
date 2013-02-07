#pragma once

#include "./Human/Resources/Material/Material.h"

class ShaderAttribute {
public:
	ShaderAttribute(MaterialPtr material, const char* attribute) 
	{ 
		_material = material; 
		_handle = material->GetAttribute(attribute);
	}

	void Enable() { _material->EnableAttribute(_handle); }
	void Disable() { _material->DisableAttribute(_handle); }

	ui32 Handle() { return _handle; }
private:
	ui32 _handle;
	MaterialPtr _material;
};
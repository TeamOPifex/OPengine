#pragma once

#include "Material.h"

class WorldMaterial : public Material {
public:
	virtual ui32 attribute_location(const char* attribute) = 0;
	virtual ui32 uniform_location(const char* attribute)= 0;
	virtual	void bind_attribute(i32 pos, char* variable) = 0;
	virtual void set_data(ui32 attribute, ui32 size, bool normalize, ui32 stride, void* arrayData) = 0;
	virtual void enable_attrib(ui32 attribute) = 0;
	virtual void set_matrix(ui32 attribute, f32* matrix) = 0;
	virtual void disable_attrib(ui32 attribute) = 0;

	ui32 WorldMatrixLocation() { return _worldMatrix; }
private:
	ui32 _worldMatrix;
};
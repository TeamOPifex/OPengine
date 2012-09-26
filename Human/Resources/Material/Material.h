#pragma once

#include "Shader\Shader.h"

class Material{
public:
	Material() { }
	ui32 handle() { return m_handle; }
	virtual ui32 attribute_location(const char* attribute) = 0;
	virtual void set_data(ui32 attribute, ui32 size, bool normalize, ui32 stride, void* arrayData) = 0;
	virtual void enable_attrib(ui32 attribute) = 0;
	virtual void disable_attrib(ui32 attribute) = 0;
protected:
	ui32 m_handle;
};
typedef Material* MaterialPtr;
#pragma once

#include "Human\Resources\Material\Material.h"

	class GLESMaterial : public Material{
	public:
		GLESMaterial() { }
		GLESMaterial(ShaderPtr vertex, ShaderPtr fragment) { load(vertex, fragment); }
		void load(ShaderPtr vertex, ShaderPtr fragment);
		ui32 attribute_location(const char* attribute);
		void set_data(ui32 attribute, ui32 size, bool normalize, ui32 stride, void* arrayData);
		void enable_attrib(ui32 attribute);
		void disable_attrib(ui32 attribute);
	};
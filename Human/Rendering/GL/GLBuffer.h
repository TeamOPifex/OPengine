#pragma once

#include "Human\Resources\Buffer\Buffer.h"

class GLBuffer : public Buffer{
public:
	GLBuffer(){ }
	GLBuffer(int shaderType, ui32 size, const f32* data){
		load(shaderType, size, data);
	}
	void load(int shaderType, ui32 size, const f32* data);
private:
	ui32 m_vertex_handle;
};
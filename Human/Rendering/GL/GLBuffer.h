#pragma once

#include "Data\Resources\Buffer\Buffer.h"

namespace OPifex{
	class GLBuffer : public Buffer{
	public:
		GLBuffer(){ }
		GLBuffer(BufferType shaderType, ui32 size, const f32* data){
			load(shaderType, size, data);
		}
		void load(BufferType shaderType, ui32 size, const f32* data);
	private:
		ui32 m_vertex_handle;
	};
}
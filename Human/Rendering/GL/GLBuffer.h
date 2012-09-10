#pragma once

#include "Data\Resources\Buffer\Buffer.h"

namespace OPifex{
	class GLBuffer : public Buffer{
	public:
		void load(BufferType shaderType, ui32 size, const f32* data);
		void enable_attribute(ui32 attribute);
		void disable_attribute(ui32 attribute);
	private:
		ui32 m_vertex_handle;
	};
}
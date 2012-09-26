#pragma once

#include "Core/Types.h"
#include "BufferTypes.h"

	class Buffer {
	public:
		virtual void load(BufferType shaderType, ui32 size, const f32* data) = 0;
		ui32 handle(){ return m_handle; }
	protected:
		ui32 m_handle;
	};
	typedef Buffer* BufferPtr;
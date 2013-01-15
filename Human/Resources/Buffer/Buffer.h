#pragma once

#include "Core/include/Types.h"
#include "BufferTypes.h"

class Buffer {
public:
	virtual void load(int shaderType, ui32 size, const void* data) = 0;
	ui32 handle(){ return m_handle; }
protected:
	ui32 m_handle;
};
typedef Buffer* BufferPtr;
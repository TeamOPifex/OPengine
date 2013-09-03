#pragma once

#include "./Human/include/Resources/Resource.h"
#include "BufferTypes.h"

class Buffer : public Resource {
public:
	Buffer();
	Buffer(BufferType shaderType, ui32 size, const void* data);
};

typedef Buffer* BufferPtr;
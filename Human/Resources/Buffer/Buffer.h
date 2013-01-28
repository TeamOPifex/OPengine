#pragma once

#include "./Human/Resources/Resource.h"
#include "BufferTypes.h"

class Buffer : public Resource {
public:
	Buffer(BufferType shaderType, ui32 size, const void* data);
};

typedef Buffer* BufferPtr;
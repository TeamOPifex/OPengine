#pragma once

#include "./Core/include/Types.h"

class Resource{
public:
	ui32 Handle() { return m_handle; }
protected:
	ui32 m_handle;
};

typedef Resource* ResourcePtr;
#pragma once

#include "Core\Types.h"

namespace OPifex{
	class ApiRenderer{
	public:
		ApiRenderer(){}
		virtual ~ApiRenderer() { }
		virtual int initialize() = 0;
		virtual void clear_color(f32 r, f32 g, f32 b) = 0;
		virtual void swap_buffer() = 0;
		virtual void shutdown() = 0;
	};
}

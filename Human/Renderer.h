#pragma once

namespace OPifex{
	class Renderer{
	public:
		Renderer(){}
		virtual ~Renderer() { }

		virtual void clear_color() = 0;
	};
}
#pragma once

namespace OPifex{
	class ApiRenderer{
	public:
		ApiRenderer(){}
		virtual ~ApiRenderer() { }
		virtual void clear_color() = 0;
	};
}

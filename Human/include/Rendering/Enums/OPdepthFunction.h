#pragma once

struct OPdepthFunction {
	enum Enum {
		LESS,
		NEVER,
		EQUAL,
		LEQUAL,
		GREATER,
		NOTEQUAL,
		GEQUAL,
		ALWAYS
	};
};
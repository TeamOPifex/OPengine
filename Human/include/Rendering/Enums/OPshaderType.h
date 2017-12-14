#pragma once

#include "./Core/include/OPtypes.h"

struct OPshaderType {
	enum Enum {
		VERTEX = 0,
		FRAGMENT
	};
};

struct OPshaderElementType {
	enum Enum {
		SHORT = 0,
		INT,
		FLOAT
	};
};

inline const OPchar* OPshaderElementTypeToStr(OPshaderElementType::Enum shaderElementType) {
	switch (shaderElementType) {
		case OPshaderElementType::SHORT: return "SHORT";
		case OPshaderElementType::INT: return "INT";
		case OPshaderElementType::FLOAT: return "FLOAT";
	}
	return "";
}
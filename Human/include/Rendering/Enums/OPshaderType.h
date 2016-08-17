#pragma once

#include "./Core/include/OPtypes.h"

enum struct OPshaderType {
	VERTEX = 0,
	FRAGMENT
};

enum struct OPshaderElementType {
	SHORT = 0,
	INT,
	FLOAT
};

inline const OPchar* OPshaderElementTypeToStr(OPshaderElementType shaderElementType) {
	switch (shaderElementType) {
		case OPshaderElementType::SHORT: return "SHORT";
		case OPshaderElementType::INT: return "INT";
		case OPshaderElementType::FLOAT: return "FLOAT";
	}
	return "";
}
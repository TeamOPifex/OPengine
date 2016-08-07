#ifndef OPENGINE_HUMAN_RENDERING_ENUMS_SHADER_TYPE
#define OPENGINE_HUMAN_RENDERING_ENUMS_SHADER_TYPE

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

#endif
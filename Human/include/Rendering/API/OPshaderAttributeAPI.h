#ifndef OPENGINE_HUMAN_RENDERING_SHADER_ATTRIBUTE
#define OPENGINE_HUMAN_RENDERING_SHADER_ATTRIBUTE

#include "./Core/include/OPtypes.h"

struct OPshaderAttributeAPI;
typedef struct OPshaderAttributeAPI OPshaderAttributeAPI;

#include "./Human/include/Rendering/Enums/OPshaderType.h"
struct OPeffect;
struct OPshaderAttribute;

struct OPshaderAttributeAPI {
	void(*SetOffset)(OPshaderAttribute* shaderAttribute, OPeffect* effect);
};

#endif
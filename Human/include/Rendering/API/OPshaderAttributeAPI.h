#pragma once

struct OPshaderAttributeAPI;
typedef struct OPshaderAttributeAPI OPshaderAttributeAPI;

#include "./Core/include/OPtypes.h"

#include "./Human/include/Rendering/Enums/OPshaderType.h"
struct OPeffect;
struct OPshaderAttribute;

struct OPshaderAttributeAPI {
	void(*SetOffset)(OPshaderAttribute* shaderAttribute, OPeffect* effect);
};
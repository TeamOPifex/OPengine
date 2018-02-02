#pragma once

struct OPmaterialParam;
typedef struct OPmaterialParam OPmaterialParam;

#include "./Human/include/Rendering/Enums/OPmaterialParamType.h"
#include "./Human/include/Rendering/OPshaderUniform.h"
#include "./Core/include/OPtypes.h"

struct OPmaterialParam {
	OPshaderUniform* uniform;
	void* data;
	ui32 loc;
};

struct OPmaterialUniformBufferParam {
	OPshaderUniformBuffer* ubo;
	OPshaderUniformBufferUniform* uniform;
	void* data;
	ui32 loc;
};
#pragma once

struct OPmaterialParam;
typedef struct OPmaterialParam OPmaterialParam;

#include "./Human/include/Rendering/Enums/OPmaterialParamType.h"
#include "./Core/include/OPtypes.h"

struct OPmaterialParam {
	OPmaterialParamType  type;
	const OPchar*        name;
	void*                data;
	ui8                  count;
};

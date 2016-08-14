#pragma once

struct OPmonitorAPI;
typedef struct OPmonitorAPI OPmonitorAPI;

struct OPmonitorResult;

struct OPmonitorAPI {
	OPmonitorResult(*GetAll)() = 0;
};
#pragma once

#include "./Data/include/OPcman.h"

#define OP_DEFAULT_LOADER_COUNT 8
extern OPassetLoader OP_DEFAULT_LOADERS[OP_DEFAULT_LOADER_COUNT];
void OPloadersAddDefault();
#pragma once

#include "./Core/include/OPtypes.h"

struct OPnetworkAddress {
	struct addrinfo* result;
	i32 iResult;
};
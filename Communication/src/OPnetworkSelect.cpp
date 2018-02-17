#include "./Communication/include/OPnetworkSelect.h"
#include "./Communication/include/OPnetworkPlatform.h"

void OPnetworkSelect::Zero() {
	FD_ZERO(&read);
	FD_ZERO(&write);
	FD_ZERO(&except);
    highest = 0;
}

i32 OPnetworkSelect::Select() {

	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 1000;

	int toRet = select(highest + 1, &read, &write, &except, &timeout);
    return toRet;
}
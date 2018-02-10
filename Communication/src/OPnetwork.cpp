#include "./Communication/include/OPnetwork.h"

#include "./Core/include/OPlog.h"


void OPnetwork::LogError(const OPchar* message) {
#ifdef OPIFEX_WINDOWS
	OPlogErr("%s: %ld", message, WSAGetLastError());
#else
	OPlogErr("%s", message);
#endif
}


i32 OPnetwork::Initialize() {
#ifdef OPIFEX_WINDOWS
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		OPlogDebug("WSAStartup failed.");
		return -1;
	}
#endif
	return 0;
}

i32 OPnetwork::Shutdown(){	
	NETWORK_CLEANUP();
	return 0;
}
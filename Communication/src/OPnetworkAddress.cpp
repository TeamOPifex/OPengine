#include "./Communication/include/OPnetworkAddress.h"
#include "./Communication/include/OPnetworkPlatform.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPmemory.h"
#include "./Data/include/OPstring.h"

void OPnetworkAddress::Init(ui32 port) {
    networkPort = port;
	OPstringCopyInto(OPstringFrom(networkPort), networkPortStr);
	valid = false;
	// OPstringCopyInto(buffer, networkAddressStr);


    struct addrinfo hints;
	OPbzero(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;//AF_INET;// Use IPv4 // AF_UNSPEC;  // use IPv4 or IPv6, whichever
	hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    addrinfo* addressInfo = NULL;
    i32 iResult = getaddrinfo(NULL, networkPortStr, &hints, &addressInfo);

	if(iResult < 0) {
	 	OPlogErr("Failed to create address");
		return;
	}

	// fill in IP Address being used

	void* addr = NULL;

	if(addressInfo->ai_family == AF_INET) {
		OPlogInfo("INET6");
		networkFamily = OPnetworkFamily::INET;
		struct sockaddr_in *ipv4 = (struct sockaddr_in *)addressInfo->ai_addr;
		addr = &(ipv4->sin_addr);
	} else if(addressInfo->ai_family == AF_INET6) {
		OPlogInfo("INET6");
		networkFamily = OPnetworkFamily::INET6;
		struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)addressInfo->ai_addr;
		addr = &(ipv6->sin6_addr);
	} else {
		networkFamily = OPnetworkFamily::UNSPEC;
	}

	if(addr != NULL) {
		OPchar buffer[128];
		inet_ntop(addressInfo->ai_family, addr, buffer, 128);
		OPlogInfo("Address: %s", buffer);
		OPstringCopyInto(buffer, networkAddressStr);
	}
	
	valid = true;
}

void OPnetworkAddress::Init(const OPchar* address, ui32 port) {
    networkPort = port;
	OPstringCopyInto(OPstringFrom(networkPort), networkPortStr);



    struct addrinfo hints;
	OPbzero(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;//AF_INET;// Use IPv4 // AF_UNSPEC;  // use IPv4 or IPv6, whichever
	hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    addrinfo* addressInfo = NULL;
    i32 iResult = getaddrinfo(address, networkPortStr, &hints, &addressInfo);



	void* addr = NULL;

	if(addressInfo->ai_family == AF_INET) {
		OPlogInfo("INET4");
		networkFamily = OPnetworkFamily::INET;
		struct sockaddr_in *ipv4 = (struct sockaddr_in *)addressInfo->ai_addr;
		addr = &(ipv4->sin_addr);
	} else if(addressInfo->ai_family == AF_INET6) {
		OPlogInfo("INET6");
		networkFamily = OPnetworkFamily::INET6;
		struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)addressInfo->ai_addr;
		addr = &(ipv6->sin6_addr);
	} else {
		networkFamily = OPnetworkFamily::UNSPEC;
	}

	if(addr != NULL) {
		OPchar buffer[128];
		inet_ntop(addressInfo->ai_family, addr, buffer, 128);
		OPlogInfo("Address: %s", buffer);
		OPstringCopyInto(buffer, networkAddressStr);
	}



    valid = true;
}

void OPnetworkAddress::Destroy() {
	
}
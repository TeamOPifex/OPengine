#include "./Communication/include/OPnetworkAddress.h"
#include "./Communication/include/OPnetworkPlatform.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPmemory.h"
#include "./Data/include/OPstring.h"

void OPnetworkAddress::Init(ui32 port, OPnetworkFamily::Enum family) {
    networkPort = port;
	OPstringCopyInto(OPstringFrom(networkPort), networkPortStr);
	valid = false;
	networkAddressStr[0] = NULL;
	valid = true;
	networkFamily = family;
}

void OPnetworkAddress::Init(const OPchar* address, ui32 port) {
    networkPort = port;
	OPstringCopyInto(OPstringFrom(networkPort), networkPortStr);



    struct addrinfo hints;
	OPbzero(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC; // use IPv4 or IPv6, whichever
	hints.ai_flags = AI_PASSIVE; // fill in my IP for me

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


void OPnetworkAddress::Init(struct sockaddr_storage* sockAddr) {
	valid = false;

	if(sockAddr->ss_family == AF_INET6) {
		struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)sockAddr;

		// Network Address
		OPchar buffer[128];
		inet_ntop(AF_INET6, &ipv6->sin6_addr, buffer, 128);
		OPstringCopyInto(buffer, networkAddressStr);
			
		// Netowrk Port
		networkPort = ipv6->sin6_port;
		OPstringCopyInto(OPstringFrom(networkPort), networkPortStr);

		// Netowrk Family
		networkFamily = OPnetworkFamily::INET6;

		valid = true;
	} else {
		struct sockaddr_in *ipv4 = (struct sockaddr_in *)sockAddr;

		// Network Address
		OPchar buffer[128];
		inet_ntop(AF_INET, &ipv4->sin_addr, buffer, 128);
		OPstringCopyInto(buffer, networkAddressStr);

		// Netowrk Port
		networkPort = ipv4->sin_port;
		OPstringCopyInto(OPstringFrom(networkPort), networkPortStr);

		// Netowrk Family
		networkFamily = OPnetworkFamily::INET;

		valid = true;
	}
}

void OPnetworkAddress::Destroy() {
	
}
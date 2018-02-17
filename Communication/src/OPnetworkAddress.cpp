#include "./Communication/include/OPnetworkAddress.h"
#include "./Communication/include/OPnetworkPlatform.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPmemory.h"
#include "./Data/include/OPstring.h"

ui64 LookupAddress(const OPchar *kpAddress, OPchar** resolved);

bool FillFromAddr(OPnetworkAddress* networkAddress) {


    addrinfo* addressInfo = networkAddress->addressInfo;

	while(!addressInfo->ai_addr && addressInfo->ai_next) {
		addressInfo = addressInfo->ai_next;
	}

	if(!addressInfo->ai_addr) {
		return false;
	}

	// Copy necessary values out of result before freeing
	
	void* addr = NULL;

	if(addressInfo->ai_family == AF_INET) {
		OPlogInfo("INET6");
		networkAddress->networkFamily = OPnetworkFamily::INET;
		struct sockaddr_in *ipv4 = (struct sockaddr_in *)addressInfo->ai_addr;
		addr = &(ipv4->sin_addr);
		networkAddress->networkAddress = ipv4->sin_addr.s_addr;

	} else if(addressInfo->ai_family == AF_INET6) {
		OPlogInfo("INET6");
		networkAddress->networkFamily = OPnetworkFamily::INET6;
		struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)addressInfo->ai_addr;
		addr = &(ipv6->sin6_addr);
		// networkAddress6->networkAddress = ipv6->sin6_addr.s_addr;

	} else {
		networkAddress->networkFamily = OPnetworkFamily::UNSPEC;
	}

	if(addr != NULL) {
		OPchar buffer[128];
		inet_ntop(addressInfo->ai_family, addr, buffer, 128);
		OPlogInfo("Address: %s", buffer);
		OPstringCopyInto(buffer, networkAddress->networkAddressStr);
	}

	if(addressInfo->ai_socktype == SOCK_STREAM) {
		networkAddress->networkSocketType = OPnetworkSocketType::STREAM;
	} else {
		networkAddress->networkSocketType = OPnetworkSocketType::DGRAM;
	}

	return true;
}

bool OPnetworkAddress::Match(sockaddr_storage* addr) {

	if(addr->ss_family == AF_INET) {
		sockaddr_in* ipv4 = (struct sockaddr_in *)addr;
        //struct sockadd_in* ipv4 = addrIn->sin_addr:

		if(networkFamily != OPnetworkFamily::INET) {
			return false;
		}

		if(networkAddress == ipv4->sin_addr.s_addr) {
			return true;
		}

	} else if(addr->ss_family == AF_INET6) {
		sockaddr_in6* ipv6= (struct sockaddr_in6*)addr;
        // struct sockadd_in6* ipv6 = ((struct sockadd_in6 *)addr)->sin6_addr;

		if(networkFamily != OPnetworkFamily::INET6) {
			return false;
		}

		void* addrData = &(ipv6->sin6_addr);
		i8 bufferAddr[128];
		inet_ntop(addr->ss_family, addr, bufferAddr, 128);

		OPlogInfo("MATCH? %s == %s", networkAddressStr, bufferAddr);
	
		if(OPstringEquals(networkAddressStr, bufferAddr)) {
			return true;
		}
	}

	return false;
}

void OPnetworkAddress::Init(sockaddr* sockaddr, OPnetworkProtocolType::Enum protocol) {

	addr = *sockaddr;

	sockaddr_in* clientSocketAddress = (sockaddr_in*)sockaddr;

	OPchar* addr = inet_ntoa(clientSocketAddress->sin_addr);
    ui16 port = ntohs(clientSocketAddress->sin_port);
	
	OPstringCopyInto(addr, networkAddressStr);
	networkPort = port;
	OPstringCopyInto(OPstringFrom(networkPort), networkPortStr);

	networkSocketType = protocol == OPnetworkProtocolType::TCP ? OPnetworkSocketType::STREAM : OPnetworkSocketType::DGRAM;
	
	if(sockaddr->sa_family == AF_INET) {
		networkFamily = OPnetworkFamily::INET;
	} else {
		networkFamily = OPnetworkFamily::INET6;
	}

    struct addrinfo hints;
	OPbzero(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever

    i32 iResult = getaddrinfo(networkAddressStr, networkPortStr, &hints, &addressInfo);
	if(iResult < 0) {
		OPlogErr("Failed to create address");
		return;
	}

	valid = true;
}

void OPnetworkAddress::Init(ui32 port, OPnetworkProtocolType::Enum protocol) {
    networkPort = port;
	networkFamily = OPnetworkFamily::INET;
	networkSocketType = protocol == OPnetworkProtocolType::TCP ? OPnetworkSocketType::STREAM : OPnetworkSocketType::DGRAM;
	OPstringCopyInto(OPstringFrom(networkPort), networkPortStr);


    struct addrinfo hints;
	OPbzero(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
	hints.ai_socktype = protocol == OPnetworkProtocolType::TCP ? SOCK_STREAM : SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    addressInfo = NULL;
    i32 iResult = getaddrinfo(NULL, networkPortStr, &hints, &addressInfo);

	if(iResult < 0) {
		OPlogErr("Failed to create address");
		return;
	}

	OPlogInfo("Fill from addr");	

	if(!FillFromAddr(this)) {
		freeaddrinfo(addressInfo);
		addressInfo = NULL;
		valid = false;
		return;
	}

	OPlogInfo("All set");
	
	valid = true;

}


void OPnetworkAddress::Init(const OPchar* address, ui32 port, OPnetworkProtocolType::Enum protocol) {
    networkPort = port;
	networkSocketType = protocol == OPnetworkProtocolType::TCP ? OPnetworkSocketType::STREAM : OPnetworkSocketType::DGRAM;
    //networkSocketType = socketType;
    valid = false;
	OPstringCopyInto(OPstringFrom(networkPort), networkPortStr);
	


    struct addrinfo hints;
	OPbzero(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;//OPnetworkFamilyTypeToCode(networkFamily);
	hints.ai_socktype = protocol == OPnetworkProtocolType::TCP ? SOCK_STREAM : SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // fill in my IP for me


	// OPchar* addr = NULL;
	// ui64 r = LookupAddress(address, &addr);
	// OPlogDebug("Address %d:%s", r, port);

	addressInfo = NULL;
    i32 iResult = getaddrinfo(address, networkPortStr, &hints, &addressInfo);

	if (iResult != 0) {
		OPlog("getaddrinfo failed: %d", iResult);

		if (iResult == EAI_AGAIN) {
			OPlog("EAI_AGAIN");
		} else if (iResult == EAI_BADFLAGS) {
			OPlog("EAI_BADFLAGS");
		} else if (iResult == EAI_FAIL) {
			OPlog("EAI_FAIL");
		} else if (iResult == EAI_FAMILY) {
			OPlog("EAI_FAMILY");
		} else if (iResult == EAI_MEMORY) {
			OPlog("EAI_MEMORY");
		} else if (iResult == EAI_NONAME) {
			OPlog("EAI_NONAME");
		}
#ifndef OPIFEX_WINDOWS
		else if (iResult == EAI_OVERFLOW) {
			OPlog("EAI_OVERFLOW");
		} else if (iResult == EAI_SYSTEM) {
			OPlog("EAI_SYSTEM");
		}
#endif
	} else {

		if(!FillFromAddr(this)) {
			freeaddrinfo(addressInfo);
			valid = false;
			return;
		}

        valid = true;
    }
}



ui64 LookupAddress(const OPchar *address, OPchar** resolved)
{
 	ui64 a;
	*resolved = NULL;

 	OPlogInfo("OPnetworkLookupAddress() - Resolving '%s'", address);

 	if ((a = inet_addr(address)) == INADDR_NONE)
 	{
 		hostent* pHE = gethostbyname(address);

 		if (pHE == 0)
 		{
 			OPlogErr("FAILED TO RESOLVE ADDRESS");
 			// Failed to resolve the address
 			return INADDR_NONE;
 		}

 		i32 i = 0;
 		struct in_addr addr;

 		OPlogInfo("Network: %s\n", pHE->h_name);
 		if (pHE->h_addrtype == AF_INET)
 		{
 			while (pHE->h_addr_list[i] != 0) {
 				addr.s_addr = *(u_long *)pHE->h_addr_list[i++];

 				OPchar* ad = inet_ntoa(addr);
 				ui32 len = (ui32)strlen(ad);
 				*resolved = (OPchar*)OPalloc(len + 1);
 				OPmemcpy(*resolved, ad, len);
 				(*resolved)[len] = NULL;

 				OPlogInfo("IP Address %s\n", ad);
 			}
 		}
	}

 	return a;
}

void OPnetworkAddress::Destroy() {
	freeaddrinfo(addressInfo);
}
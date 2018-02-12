#include "./Communication/include/OPnetworkAddress.h"
#include "./Communication/include/OPnetworkPlatform.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPmemory.h"
#include "./Data/include/OPstring.h"

ui64 LookupAddress(const OPchar *kpAddress, OPchar** resolved);


void OPnetworkAddress::Init(sockaddr_in* sockaddr_in) {
    networkAddress = inet_ntoa(sockaddr_in->sin_addr);
    networkPort = ntohs(sockaddr_in->sin_port);
	networkFamily = OPnetworkFamily::INET;// TODO: sockaddr_in->sin_family;
	OPstringCopyInto(OPstringFrom(networkPort), networkPortStr);

	OPbzero(&sockAddr, sizeof(sockAddr));
	SockAddrIn()->sin_family = AF_INET;
	SockAddrIn()->sin_addr.s_addr = sockaddr_in->sin_addr.s_addr;
	SockAddrIn()->sin_port = htons(networkPort);

	valid = true;
}

void OPnetworkAddress::Init(ui32 port, OPnetworkFamily::Enum family) {
    networkAddress = "127.0.0.1";
    networkPort = port;
	networkFamily = family;
	OPstringCopyInto(OPstringFrom(networkPort), networkPortStr);


	OPbzero(&sockAddr, sizeof(sockAddr));
	SockAddrIn()->sin_family = AF_INET;
	SockAddrIn()->sin_addr.s_addr = htonl(INADDR_ANY);
	SockAddrIn()->sin_port = htons(port);

	valid = true;

}

void OPnetworkAddress::Init(const OPchar* address, ui32 port, OPnetworkFamily::Enum family) {
    networkAddress = address;
    networkPort = port;
	networkFamily = family;
    //networkSocketType = socketType;
    valid = false;
	OPstringCopyInto(OPstringFrom(networkPort), networkPortStr);
	


    struct addrinfo hints;
	OPbzero(&hints, sizeof(hints));
	hints.ai_family = OPnetworkFamilyTypeToCode(family);
	// hints.ai_socktype = socketType == OPnetworkSocketType::TCP ? SOCK_STREAM : SOCK_DGRAM;
	// hints.ai_protocol = IPPROTO_UDP;



	// OPchar* addr = NULL;
	// ui64 r = LookupAddress(address, &addr);
	// OPlogDebug("Address %d:%s", r, port);

    addrinfo* result;
    //i32 iResult = getaddrinfo(addr == NULL ? address : addr, port, &hints, &result);
	result = NULL;
    i32 iResult = getaddrinfo(address, networkPortStr, &hints, &result);

	// if (addr != NULL) {
	// 	OPfree(addr);
	// }

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

		while(!result->ai_addr && result->ai_next) {
			result = result->ai_next;
		}

		if(!result->ai_addr) {
        	freeaddrinfo(result);
			result = NULL;
			valid = false;
			return;
		}

		// Copy necessary values out of result before freeing

        if(result->ai_family == AF_INET) {
            networkFamily = OPnetworkFamily::INET;
        } else if(result->ai_family == AF_INET6) {
            networkFamily = OPnetworkFamily::INET6;
        } else {
            networkFamily = OPnetworkFamily::UNSPEC;
        }

        if(result->ai_socktype == SOCK_STREAM) {
            networkSocketType = OPnetworkSocketType::STREAM;
        } else {
            networkSocketType = OPnetworkSocketType::DGRAM;
        }

		sockAddr = *result->ai_addr;
		
        freeaddrinfo(result);


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
#include "./Communication/include/OPnetworkAddress.h"
#include "./Communication/include/OPnetworkPlatform.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPmemory.h"

ui64 LookupAddress(const OPchar *kpAddress, OPchar** resolved);

/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.

	*/
char* itoa(int value, char* result, int base) {
	// check that the base if valid
	if (base < 2 || base > 36) { *result = '\0'; return result; }

	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while ( value );

	// Apply negative sign
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}

void OPnetworkAddress::Init(const OPchar* address, ui32 port, OPnetworkSocketType::Enum socketType) {
	i8 portStr[6];
	itoa(port, portStr, 10);
	Init(address, portStr, socketType);
}

void OPnetworkAddress::Init(const OPchar* address, const OPchar* port, OPnetworkSocketType::Enum socketType) {
    networkAddress = address;
    networkPort = port;
	networkPortNum = atoi(port);
    networkSocketType = socketType;
    valid = false;


    struct addrinfo hints;

	OPbzero(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = socketType == OPnetworkSocketType::TCP ? SOCK_STREAM : SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;


	OPchar* addr = NULL;
	ui64 r = LookupAddress(address, &addr);
	OPlogDebug("Address %d:%s", r, port);

    
    i32 iResult = getaddrinfo(addr == NULL ? address : addr, port, &hints, &result);

	if (addr != NULL) {
		OPfree(addr);
	}

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
        if(result->ai_family == AF_INET) {
            networkFamily = OPnetworkFamily::INET;
        } else if(result->ai_family == AF_INET6) {
            networkFamily = OPnetworkFamily::INET6;
        } else {
            networkFamily = OPnetworkFamily::UNSPEC;
        }

        if(result->ai_socktype == SOCK_STREAM) {
            networkSocketType = OPnetworkSocketType::TCP;
        } else {
            networkSocketType = OPnetworkSocketType::UDP;
        }

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
#pragma once

#include "./Communication/include/OPnetwork.h"
#include "./Communication/include/OPnetworkSocket.h"

struct OPnetworkSelect {
    fd_set read;
    fd_set write;
    fd_set except;
    i32 highest;

    void Zero();
    i32 Select();

    inline void SetHighest(OPnetworkSocket* networkSocket) {
        if(highest < networkSocket->connectedSocket) {
            highest = networkSocket->connectedSocket;
        }
    }

    inline void SetRead(OPnetworkSocket* networkSocket) {
	    FD_SET(networkSocket->connectedSocket, &read);
        SetHighest(networkSocket);
    }

    inline void SetWrite(OPnetworkSocket* networkSocket) {
	    FD_SET(networkSocket->connectedSocket, &write);
        SetHighest(networkSocket);
    }
    
    inline void SetExcept(OPnetworkSocket* networkSocket) {
	    FD_SET(networkSocket->connectedSocket, &except);
        SetHighest(networkSocket);
    }

    inline bool IsReadSet(OPnetworkSocket* networkSocket) {
        return FD_ISSET(networkSocket->connectedSocket, &read);
    }

    inline bool IsWriteSet(OPnetworkSocket* networkSocket) {
        return FD_ISSET(networkSocket->connectedSocket, &write);
    }

    inline bool IsExceptSet(OPnetworkSocket* networkSocket) {
        return FD_ISSET(networkSocket->connectedSocket, &except);
    }
    
};
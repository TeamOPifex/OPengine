#include "../include/Connection.h"

OPconnection* OPhost(OPint port, OPprotocol protocol, int maxIncomming){
	OPconnection* con = (OPconnection*)OPalloc(sizeof(OPconnection));
	SockAddr *s_in;
	if(con){
		// open a socket
		con->_socket = socket(AF_INET, SOCK_DGRAM, 0);
		s_in = &(con->_sin);
		
		// setup for the specified protocol
		switch(protocol){
			case UDP:
				// create a destination
				bzero((i8*)s_in, sizeof(SockAddr)); // set everything to binary 0's
				s_in->sin_family = (i16)AF_INET;
				s_in->sin_addr.s_addr = htonl(INADDR_ANY); // THIS internet address
				s_in->sin_port = htons((ui16)port); // network byte order
				break;
			case TCP:
				break;
		}

		// bind socket to a port
		if(bind(con->_socket, (const sockaddr*)s_in, sizeof(sockaddr)) < 0){
			switch(errno){
				case EADDRINUSE:
					printf("Address is already in use.\n");
					break;
				case EACCES:
					printf("Address is protected.\n");
					break;
			}
			printf("(%d)Error binding to %d\n", errno, port);
		}

		return con;
	}
	else
		return NULL;
}

OPconnection* OPconnect(OPchar* address, OPint port, OPprotocol protocol){
	OPconnection* con = (OPconnection*)malloc(sizeof(OPconnection));
	SockAddr *s_in;

	if(con){
		con->_socket = socket(AF_INET, SOCK_DGRAM, 0);
		s_in = &(con->_sin);

		switch(protocol){
			case UDP:
				bzero((i8*) s_in, sizeof(SockAddr)); // Set entire struct to binary 0's
				HostEnt *hostptr = gethostbyname(address);
				if (hostptr){
					s_in->sin_family = (i16) AF_INET;
					bcopy(hostptr->h_addr, (i8*)&s_in->sin_addr, hostptr->h_length);
					s_in->sin_port = htons(port);
				}
				else
					exit(1);
				break;
			case TCP:
				break;
		}

		return con;
	}

	return NULL;
}

OPint OPreceive(OPconnection* con, void* data, OPuint size){
	SockAddr from;

	OPint sret = recv(
		con->_socket,
		(void*)data,
		size,
		0
	);

	return sret;
}

OPint OPsend(OPconnection* con, void* data, OPuint size){
	// send the datagram (destination determined in call)
	OPint sret = sendto(
		con->_socket,         // file descriptor
		data,                 // message
		size,                 
		0,                    // flags
		(sockaddr*)&con->_sin,
		sizeof(sockaddr)      // destination
	);

	return sret;
}

OPuint OPdisconnect(OPconnection* connection){
	if(shutdown(connection->_socket, SHUT_RDWR)){
		free(connection);
		return 1;
	}
	return 0;
}

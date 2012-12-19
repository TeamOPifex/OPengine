#include <stdio.h>
#include <string.h>
#include "include/Connection.h"

typedef struct{
	char txt[100];
} Message;

int main(int argc, char** argv){
	OPconnection* con;

	if(atoi(argv[1]) == 0){
		Message msg;
		printf("Hosting\n");
		con = OPhost(1337, UDP, 10);
		while(1){
			if(OPreceive(con, &msg, sizeof(Message))){
				printf("Message: ");
				printf("%s\n", msg.txt);
				OPdisconnect(con);
				break;
			}
			
		}
	}
	else if(atoi(argv[1]) == 1){
		int i = 0;
		Message msg;
		for(i = 0; i < 100; i++) msg.txt[i] = '\0';

		printf("Joining %s...\n", argv[2]);
		con = OPconnect(argv[2], 1337, UDP);
		
		strncpy(msg.txt, argv[3], strlen(argv[3]));		
		OPsend(con, &msg, sizeof(Message));
	}

	

	return 0;
}

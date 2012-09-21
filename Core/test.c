#include <stdio.h>
#include "Core.h"

sint main(){	
	sint* i = (sint*)OPalloc(sizeof(sint));
	*i = 1337;

	printf("Int: %d Size: %d\n", *i, sizeof(sint));
	//printf("%f, %f, %f\n", v.x,v.y,v.z); 
	OPfree(i);

	return 0;
}

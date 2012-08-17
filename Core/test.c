#include <stdio.h>
#include "Core.h"

int main(){	
	i32* i = (i32*)OPalloc(sizeof(i32));
	*i = 1337;

	printf("Int32: %d\n", *i);
	//printf("%f, %f, %f\n", v.x,v.y,v.z); 
	OPfree(i);

	return 0;
}

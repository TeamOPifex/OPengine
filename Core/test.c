#include <stdio.h>
#include "Core.h"

OPint main(){
	OPint* i = (OPint*)OPalloc(sizeof(OPint));
	*i = 1337;

	printf("Int: %d Size: %d\n", *i, sizeof(OPint));
	printf("PI = %f\n", OPpi); 
	OPfree(i);

	printf("i = %d\n", (OPint)i);
	
	OPfloat pi = -21;//OPpi;
	printf("-pi = %f\nabs(-pi) = %f\nsin(-pi) = %f\nceil(-pi) = %d\n", pi, (OPfloat)OPabs(pi), (OPfloat)OPsin(pi), (OPint)OPceil(pi));
	
	printf("Rand 0 - 1000: %f\n", OPrandRange(0, 1000));
	printf("Random: %f\n", OPrandom());	

	return 0;
}

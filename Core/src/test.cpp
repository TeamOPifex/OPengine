#include <stdio.h>
#include "./../include/Core.h"

using namespace std;
using namespace OPEngine::Core;

class TestCore : public virtual OPCore{
	public:
		TestCore() {
			i = NULL;
		}
		void Initialize(){
			i = (OPint*)OPalloc(sizeof(OPint));
			*i = 1337;
			printf("Int: %d Size: %d\n", *i, sizeof(OPint));
			printf("PI = %f\n", OPpi); 


			//printf("i = %d\n", (OPint)i);

			OPfloat pi = -21;//OPpi;
			printf("-pi = %f\nabs(-pi) = %f\nsin(-pi) = %f\nceil(-pi) = %d\n", pi, (OPfloat)OPabs(pi), (OPfloat)OPsin(pi), (OPint)OPceil(pi));
			
			printf("Rand 0 - 1000: %f\n", OPrandRange(0, 1000));
			printf("Random: %f\n", OPrandom());
			*i = 0;
		}
		//----------------------------------------------------------------------------
		void Update(OPtimer* timer){
			if(*i){
				printf("Seconds: %f\n", OPtimerDelta(timer));
				End();
			}

			*i = (OPint)getchar();
		}
		//----------------------------------------------------------------------------
		void Destroy(){
			OPfree(i);
		}
	private:
		OPint* i;
};
//----------------------------------------------------------------------------
OPint main(){
	TestCore* core = new TestCore();
	
	// start the game cycle
	core->Start();

	return 0;
}


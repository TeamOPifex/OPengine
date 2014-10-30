#include "./../include/MathHelpers.h"

OPint _isRand = 0;

//----------------------------------------------------------------------------
OPfloat OPrandom(){
	#if defined(OPIFEX_UNIX) // linux implementation
		if(!_isRand){ srand(time(NULL)); _isRand = 1; }
		return rand() / (OPfloat)RAND_MAX;
	#elif defined(OPIFEX_WINDOWS) 	// windows implementation
		if (!_isRand){ srand(time(NULL)); _isRand = 1; }
		return rand() / (OPfloat)RAND_MAX;
	#endif
}

//----------------------------------------------------------------------------
OPfloat OPrandRange(OPfloat min, OPfloat max){
	#if defined(OPIFEX_UNIX) // linux implementation
		max -= min;
		return (OPfloat)(OPrandom() * max) + min;
	#elif defined(OPIFEX_WINDOWS) 	// windows implementation
		max -= min;
		return (OPfloat)(OPrandom() * max) + min;
	#endif
}
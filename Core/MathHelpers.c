#include "MathHelpers.h"

OPint OPceil(OPfloat f){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)	// linux implementation
		return (OPint)ceil(f);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation

	#elif defined(OPIFEX_ANDROID)	// android implementation

	#endif
	return 0;
}
OPint OPfloor(OPfloat f){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)	// linux implementation
		return (OPint)floor(f);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation

	#elif defined(OPIFEX_ANDROID)	// android implementation

	#endif
	return 0;
}

OPfloat OPabsf(OPfloat f){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)	// linux implementation
		return OPabs(f);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation

	#elif defined(OPIFEX_ANDROID)	// android implementation

	#endif
	return 0;
}
OPint _isRand = 0;

OPfloat OPabs(OPfloat f){
	return f > 0 ? f : -f;
}

OPint	OPabsi(OPint i){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)	// linux implementation
		return (OPint)OPabs(i);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation

	#elif defined(OPIFEX_ANDROID)	// android implementation

	#endif
	return 0;
}

OPfloat OPsin(OPfloat f){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)	// linux implementation
		return sin(f);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation

	#elif defined(OPIFEX_ANDROID)	// android implementation

	#endif
	return 0;
}
OPfloat OPcos(OPfloat f){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)	// linux implementation
		return cos(f);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation

	#elif defined(OPIFEX_ANDROID)	// android implementation

	#endif
	return 0;
}
OPfloat OPtan(OPfloat f){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)	// linux implementation
		return tan(f);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation

	#elif defined(OPIFEX_ANDROID)	// android implementation

	#endif
	return 0;
}

OPfloat OPasin(OPfloat f){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)	// linux implementation
		return asin(f);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation

	#elif defined(OPIFEX_ANDROID)	// android implementation

	#endif
	return 0;
}
OPfloat OPacos(OPfloat f){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)	// linux implementation
		return acos(f);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation

	#elif defined(OPIFEX_ANDROID)	// android implementation

	#endif
	return 0;
}
OPfloat OPatan(OPfloat f){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)	// linux implementation
		return atan(f);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation

	#elif defined(OPIFEX_ANDROID)	// android implementation

	#endif
	return 0;
}

OPfloat OPpow(OPfloat b, OPfloat exp){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)	// linux implementation
		return pow(b, exp);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation

	#elif defined(OPIFEX_ANDROID)	// android implementation

	#endif
	return 0;
}
OPfloat OPsqrt(OPfloat f){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)	// linux implementation
		return sqrt(f);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation

	#elif defined(OPIFEX_ANDROID)	// android implementation

	#endif
	return 0;
}
OPfloat OPlog10(OPfloat f){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)	// linux implementation
		return log10(f);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation

	#elif defined(OPIFEX_ANDROID)	// android implementation

	#endif
	return 0;
}
OPfloat OPlog2(OPfloat f){
	return OPlog10(f) / OPlog10(2);
}
OPfloat OPln(OPfloat f){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)	// linux implementation
		return log(f);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation

	#elif defined(OPIFEX_ANDROID)	// android implementation

	#endif
	return 0;
}

OPfloat OPrandom(){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)	// linux implementation
		if(!_isRand){ srand(time(NULL)); _isRand = 1; }
		return rand() / (OPfloat)RAND_MAX;
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation

	#elif defined(OPIFEX_ANDROID)	// android implementation

	#endif
	return 0;
}
OPfloat OPrandRange(OPfloat min, OPfloat max){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)	// linux implementation
		max -= min;
		return (OPfloat)(OPrandom() * max);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation

	#elif defined(OPIFEX_ANDROID)	// android implementation

	#endif
	return 0;
}

#include "./../include/MathHelpers.h"

OPint OPceil(OPfloat f){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID) // linux implementation
		return (OPint)ceil(f);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation
		return (OPint)ceil(f);	
	#endif
}
//----------------------------------------------------------------------------
OPint OPfloor(OPfloat f){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID) // linux implementation
		return (OPint)floor(f);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation
		return (OPint)floor(f);
	#endif
}
//----------------------------------------------------------------------------
OPfloat OPabsf(OPfloat f){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID) // linux implementation
		return OPabs(f);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation
		return OPabs(f);
	#endif
}
OPint _isRand = 0;
//----------------------------------------------------------------------------
OPfloat OPabs(OPfloat f){
	return f > 0 ? f : -f;
}
//----------------------------------------------------------------------------
OPint	OPabsi(OPint i){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID) // linux implementation
		return (OPint)OPabs(i);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation
		return (OPint)OPabs(i);
	#endif
}
//----------------------------------------------------------------------------
OPfloat OPsin(OPfloat f){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)	// linux implementation
		return sin(f);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation
		return sin(f);
	#elif defined(OPIFEX_ANDROID)	// android implementation
		return sin(f);
	#endif
}
//----------------------------------------------------------------------------
OPfloat OPcos(OPfloat f){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)	// linux implementation
		return cos(f);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation
		return cos(f);
	#elif defined(OPIFEX_ANDROID)	// android implementation
		return cos(f);
	#endif
}
//----------------------------------------------------------------------------
OPfloat OPtan(OPfloat f){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)	// linux implementation
		return tan(f);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation
		return tan(f);
	#elif defined(OPIFEX_ANDROID)	// android implementation
		return tan(f);
	#endif
}
//----------------------------------------------------------------------------
OPfloat OPasin(OPfloat f){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID) // linux implementation
		return asin(f);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation
		return asin(f);
	#endif
}
//----------------------------------------------------------------------------
OPfloat OPacos(OPfloat f){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID) // linux implementation
		return acos(f);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation
		return acos(f);
	#endif
}
//----------------------------------------------------------------------------
OPfloat OPatan(OPfloat f){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID) // linux implementation
		return atan(f);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation
		return atan(f);
	#endif
}
//----------------------------------------------------------------------------
OPfloat OPpow(OPfloat b, OPfloat exp){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID) // linux implementation
		return pow(b, exp);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation
		return pow(b, exp);
	#endif
}
//----------------------------------------------------------------------------
OPfloat OPsqrt(OPfloat f){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID) // linux implementation
		return sqrt(f);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation
		return sqrt(f);
	#endif
}
//----------------------------------------------------------------------------
OPfloat OPlog10(OPfloat f){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID) // linux implementation
		return log10(f);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation
		return log10(f);
	#endif
}
//----------------------------------------------------------------------------
OPfloat OPlog2(OPfloat f){
	return OPlog10(f) / OPlog10(2);
}
//----------------------------------------------------------------------------
OPfloat OPln(OPfloat f){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID) // linux implementation
		return log(f);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation
		return log(f);
	#endif
}
//----------------------------------------------------------------------------
OPfloat OPrandom(){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID) // linux implementation
		if(!_isRand){ srand(time(NULL)); _isRand = 1; } // And this is doing??
		return rand() / (OPfloat)RAND_MAX;
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation
		return rand() / (OPfloat)RAND_MAX;
	#endif
}
//----------------------------------------------------------------------------
OPfloat OPrandRange(OPfloat min, OPfloat max){
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID) // linux implementation
		max -= min;
		return (OPfloat)(OPrandom() * max) + min;
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation
		max -= min;
		return (OPfloat)(OPrandom() * max) + min;
	#endif
}
//----------------------------------------------------------------------------
OPfloat OPround(OPfloat f){
#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID) // linux implementation
	return round(f);
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows implementation
	return OPfloor( f + 0.5f );
#endif
}
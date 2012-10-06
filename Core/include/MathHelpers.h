#pragma once
#ifndef OPEngine_Core_MathHelpers
#define OPEngine_Core_MathHelpers

#include "Target.h"
#include "Types.h"

#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID)	// linux math libs
#include <math.h>
#include <stdlib.h>
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows math libs
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#endif

#define OPpi 3.14159265359
#define OPe  2.71828182846

extern OPint OPceil(OPfloat f);
extern OPint OPfloor(OPfloat f);

extern OPfloat OPabsf(OPfloat f);
extern OPfloat OPabs(OPfloat f);
extern OPint	OPabsi(OPint i);

extern OPfloat OPsin(OPfloat f);
extern OPfloat OPcos(OPfloat f);
extern OPfloat OPtan(OPfloat f);

extern OPfloat OPasin(OPfloat f);
extern OPfloat OPacos(OPfloat f);
extern OPfloat OPatan(OPfloat f);

extern OPfloat OPpow(OPfloat b, OPfloat exp);
extern OPfloat OPsqrt(OPfloat f);
extern OPfloat OPlog10(OPfloat f);
extern OPfloat OPlog2(OPfloat f);
extern OPfloat OPln(OPfloat f);

extern OPfloat OPrandom();
extern OPfloat OPrandRange(OPfloat min, OPfloat max);
#endif

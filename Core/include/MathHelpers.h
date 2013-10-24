#pragma once
#ifndef OPEngine_Core_MathHelpers
#define OPEngine_Core_MathHelpers

#include "Types.h"

#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID)	// linux math libs
#include <math.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h> // link with -lrt
#include <time.h>
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64) 	// windows math libs
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#endif

#define OPpi 3.14159265359
#define OPe  2.71828182846

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif
/**
 * OPceil - math helper
 *	Calculates the ceiling of a floating point number. For example
 *	if 0.5 is passed as f, then 1 will be returned.
 * @param f Floating point number.
 * @return integer ceiling
 */
OPint OPceil(OPfloat f);
//-----------------------------------------------------------------------------
/**
 * OPfloor - math helper
 *	Calculates the floor of a floating point number. For example
 *	if 0.5 is passed as f, then 0 will be returned.
 * @param f Floating point number.
 * @return integer floor
 */
OPint OPfloor(OPfloat f);
//-----------------------------------------------------------------------------
/**
 * OPabsf - math helper
 *	Explicitly calculates the absolute value of a floating point number.
 * @param f Floating point number.
 * @return absolute value
 */
OPfloat OPabsf(OPfloat f);
//-----------------------------------------------------------------------------
/**
 * OPabs - math helper
 *	Calculates the absolute value of a floating point number.
 * @param f Floating point number.
 * @return absolute value
 */
OPfloat OPabs(OPfloat f);

//-----------------------------------------------------------------------------
/**
 * OPabsi - math helper
 *	Calculates the absolute value of an integer number.
 * @param i integer number.
 * @return absolute value
 */
OPint	OPabsi(OPint i);
//-----------------------------------------------------------------------------
/**
 * OPsin - math helper
 *	Calculates the sine of a floating point number.
 * @param f Floating point number.
 * @return sin(f)
 */
OPfloat OPsin(OPfloat f);
//-----------------------------------------------------------------------------
/**
 * OPcos - math helper
 *	Calculates the cosine of a floating point number.
 * @param f Floating point number.
 * @return cos(f)
 */
OPfloat OPcos(OPfloat f);
//-----------------------------------------------------------------------------
/**
 * OPtan - math helper
 *	Calculates the tangent of a floating point number.
 * @param f Floating point number.
 * @return tan(f)
 */
OPfloat OPtan(OPfloat f);

//-----------------------------------------------------------------------------
/**
 * OPasin - math helper
 *	Calculates the arc sine of a floating point number.
 * @param f Floating point number.
 * @return asin(f)
 */
OPfloat OPasin(OPfloat f);
//-----------------------------------------------------------------------------
/**
 * OPacos - math helper
 *	Calculates the arc cosine of a floating point number.
 * @param f Floating point number.
 * @return acos(f)
 */
OPfloat OPacos(OPfloat f);
//-----------------------------------------------------------------------------
/**
 * OPatan - math helper
 *	Calculates the arc tangent of a floating point number.
 * @param f Floating point number.
 * @return atan(f)
 */
OPfloat OPatan(OPfloat f);

//-----------------------------------------------------------------------------
/**
 * OPpow - math helper
 *	Raises b to a power.
 * @param b base value
 * @param exp power
 * @return b^exp
 */
OPfloat OPpow(OPfloat b, OPfloat exp);
//-----------------------------------------------------------------------------
/**
 * OPsqrt - math helper
 *	Calculates the square root of a floating point number.
 * @param f Floating point number.
 * @return square of f
 */
OPfloat OPsqrt(OPfloat f);
//-----------------------------------------------------------------------------
/**
 * OPlog10 - math helper
 *	Calculates the base 10 log of a floating point number.
 * @param f Floating point number.
 * @return log10(f)
 */
OPfloat OPlog10(OPfloat f);
//-----------------------------------------------------------------------------
/**
 * OPlog2 - math helper
 *	Calculates the base 2 log of a floating point number.
 * @param f Floating point number.
 * @return log2(f)
 */
OPfloat OPlog2(OPfloat f);
//-----------------------------------------------------------------------------
/**
 * OPln - math helper
 *	Calculates the natural log of a floating point number.
 * @param f Floating point number.
 * @return ln(f)
 */
OPfloat OPln(OPfloat f);

//-----------------------------------------------------------------------------
/**
 * OPrandom - math helper
 *	Returns a randomized floating point number.
 * @return Floating point value with the range(0, 1)
 */
OPfloat OPrandom();
//-----------------------------------------------------------------------------
/**
 * OPrandom - math helper
 *	Returns a randomized floating point number.
 * @param min Smallest possible number which can be generated
 * @param max Largest possible number which can be generated
 * @return Floating point value with the range(min, max)
 */
OPfloat OPrandRange(OPfloat min, OPfloat max);
//-----------------------------------------------------------------------------
/**
 * OPnear - math helper
 *	Determines if a value is within a threashold of another
 * @param value Value to check
 * @param target Target which value is checked against
 * @return True if near is within theshhold of target, otherwise false.
 */
__inline OPfloat OPnear(OPfloat value, OPfloat target, OPfloat threshhold){
	return (OPabs(value - target) < threshhold ? 1 : 0);
}

#ifdef __cplusplus
}
#endif
#endif

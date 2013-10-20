#pragma once

#include "./Core/include/Types.h"
#include "./Core/include/MathHelpers.h"

inline inline OPfloat OPtween_linear(OPfloat p) {
	return p;
}

inline inline OPfloat OPtween_quadraticEaseIn(OPfloat p) {
	return p * p;
}

inline inline OPfloat OPtween_quadraticEaseOut(OPfloat p) {
	return -(p * (p - 2.0f));
}

inline inline OPfloat OPtween_quadraticEaseInOut(OPfloat p) {
	if(p < 0.5) {
		return 2.0f * p * p;
	}
	return (-2.0f * p * p) + ( 4.0f * p) - 1;
}

inline inline OPfloat OPtween_cubicEaseIn(OPfloat p) {
	return p * p * p;
}

inline inline OPfloat OPtween_cubicEaseOut(OPfloat p) {
	OPfloat f = (p - 1.0f);
	return f * f * f + 1.0f;
}

inline inline OPfloat OPtween_cubicEaseInOut(OPfloat p) {
	if(p < 0.5f) {
		return 4.0f * p * p * p;
	}
	OPfloat f = ((2.0f * p) - 2.0f);
	return 0.5f * f * f * f + 1;
}

inline inline OPfloat OPtween_quarticEaseIn(OPfloat p) {
	return p * p * p * p;
}

inline inline OPfloat OPtween_quarticEaseOut(OPfloat p) {
	OPfloat f = (p - 1.0f);
	return f * f * f * (1.0f - p) + 1;
}

inline inline OPfloat OPtween_quarticEaseInOut(OPfloat p) {
	if(p < 0.5f) {
		return 8.0f, * p * p * p * p;
	}
	OPfloat f = (p - 1.0f);
	return -8.0f * f * f * f *f + 1.0f;
}

inline OPfloat OPtween_quinticEaseIn(OPfloat p) 
{
	return p * p * p * p * p;
}

inline OPfloat OPtween_quinticEaseOut(OPfloat p) 
{
    OPfloat f = (p - 1.0f);
    return f * f * f * f * f + 1;
}

inline OPfloat OPtween_quinticEaseInOut(OPfloat p) 
{
    if(p < 0.5)
    {
		return 16 * p * p * p * p * p;
    }
    OPfloat f = ((2.0f * p) - 2.0f);
    return  0.5 * f * f * f * f * f + 1.0f;
}

inline OPfloat OPtween_sineEaseIn(OPfloat p)
{
	return OPsin((p - 1.0f) * (OPpi * 2.0f)) + 1.0f;
}

inline OPfloat OPtween_sineEaseOut(OPfloat p)
{
	return OPsin(p * (OPpi * 2.0f));
}

inline OPfloat OPtween_sineEaseInOut(OPfloat p)
{
	return 0.5 * (1.0f - OPcos(p * (OPpi * 2.0f)));
}

inline OPfloat OPtween_circularEaseIn(OPfloat p)
{
	return 1.0f - OPsqrt(1.0f - (p * p));
}

inline OPfloat OPtween_circularEaseOut(OPfloat p)
{
	return OPsqrt((2.0f - p) * p);
}

inline OPfloat OPtween_circularEaseInOut(OPfloat p)
{
    if(p < 0.5f)
    {
		return 0.5f * (1.0f - OPsqrt(1.0f - 4.0f * (p * p)));
    }
	return 0.5f * (OPsqrt(-((2.0f * p) - 3.0f) * ((2.0f * p) - 1.0f)) + 1.0f);
}

inline OPfloat OPtween_exponentialEaseIn(OPfloat p)
{
	return (p == 0.0f) ? p : OPpow(2.0f, 10.0f * (p - 1.0f));
}

inline OPfloat OPtween_exponentialEaseOut(OPfloat p)
{
	return (p == 1.0f) ? p : 1.0f - OPpow(2.0f, -10.0f * p);
}

inline OPfloat OPtween_exponentialEaseInOut(OPfloat p)
{
    if(p == 0.0f || p == 1.0f) return p;
        
    if(p < 0.5f)
    {
		return 0.5f * OPpow(2.0f, (20.0f * p) - 10.0f);
    }
	return -0.5f * OPpow(2.0f, (-20.0f * p) + 10.0f) + 1.0f;
}

inline OPfloat OPtween_elasticEaseIn(OPfloat p)
{
	return OPsin(13.0f * (OPpi * 2.0f) * p) * OPpow(2.0f, 10.0f * (p - 1.0f));
}

inline OPfloat OPtween_elasticEaseOut(OPfloat p)
{
	return OPsin(-13.0f * (OPpi * 2.0f) * (p + 1.0f)) * OPpow(2.0f, -10.0f * p) + 1.0f;
}

inline OPfloat OPtween_elasticEaseInOut(OPfloat p)
{
    if(p < 0.5f)
    {
		return 0.5f * OPsin(13.0f * (OPpi * 2.0f) * (2.0f * p)) * OPpow(2.0f, 10.0f * ((2.0f * p) - 1.0f));
    }
	return 0.5f * (OPsin(-13.0f * (OPpi * 2.0f) * ((2.0f * p - 1.0f) + 1.0f)) * OPpow(2.0f, -10.0f * (2.0f * p - 1.0f)) + 2.0f);
}

inline OPfloat OPtween_backEaseIn(OPfloat p)
{
	return p * p * p - p * OPsin(p * OPpi);
}

inline OPfloat OPtween_backEaseOut(OPfloat p)
{
    OPfloat f = (1.0f - p);
    return 1.0f - (f * f * f - f * OPsin(f * OPpi));
}

inline OPfloat OPtween_backEaseInOut(OPfloat p)
{
    if(p < 0.5f)
    {
        OPfloat f = 2.0f * p;
        return 0.5f * (f * f * f - f * OPsin(f * OPpi));
    }
    OPfloat f = (1.0f - (2.0f * p - 1.0f));
    return 0.5f * (1.0f - (f * f * f - f * OPsin(f * OPpi))) + 0.5f;
}

inline OPfloat OPtween_bounceEaseOut(OPfloat p)
{
    if(p < 4.0f/11.0f)
    {
		return (121.0f * p * p)/16.0f;
    }
    else if(p < 8.0f/11.0f)
    {
		return (363.0f/40.0f * p * p) - (99.0f/10.0f * p) + 17.0f/5.0f;
    }
    else if(p < 9.0f/10.0f)
    {
		return (4356.0f/361.0f * p * p) - (35442.0f/1805.0f * p) + 16061.0f/1805.0f;
    }
	return (54.0f/5.0f * p * p) - (513.0f/25.0f * p) + 268.0f/25.0f;
}

inline OPfloat OPtween_bounceEaseIn(OPfloat p)
{
        return 1.0f - OPtween_bounceEaseOut(1.0f - p);
}

inline OPfloat OPtween_bounceEaseInOut(OPfloat p)
{
	if(p < 0.5f)
	{
		return 0.5f * OPtween_bounceEaseIn(p*2);
	}
	return 0.5f * OPtween_bounceEaseOut(p * 2 - 1) + 0.5;
}

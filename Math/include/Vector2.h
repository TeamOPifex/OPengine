#pragma once
#include "Core/include/Types.h"
#include "./Core/include/MathHelpers.h"

typedef struct {
	OPfloat x, y;
} OPvec2;

inline void vec2add(OPvec2* dst, OPvec2* a, OPvec2* b);

inline void vec2sub(OPvec2* dst, OPvec2* a, OPvec2* b);

inline void vec2mul(OPvec2* dst, OPvec2* a, OPvec2* b);

inline void vec2scl(OPvec2* dst, OPfloat s);

inline void vec2div(OPvec2* dst, OPvec2* a, OPvec2* b);

inline void vec2dot(OPfloat* dst, OPvec2* a, OPvec2* b);

inline void vec2norm(OPvec2* dst, OPvec2* a);

inline void vec2perp(OPvec2* dst, OPvec2* a);
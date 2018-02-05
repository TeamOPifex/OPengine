#pragma once

struct OPtextureFormat
{
	enum Enum {
		NONE = 0,
		RGB,
		RGBA,
		RGBA8,
		LUMINANCE,
		LUMINANCE_ALPHA,
		DEPTH,
		DEPTH16F,
		DEPTH32F,
		RGB16F,
		RGB32F,
		RGBA16F,
		RGBA32F,
		BGRA,
		BGR,
		RED,
		RG,
		RG16F
	};
};
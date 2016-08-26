#pragma once

struct OPtextureDesc;
typedef struct OPtextureDesc OPtextureDesc;

#include "./Human/include/Rendering/Enums/OPtextureFilter.h"
#include "./Human/include/Rendering/Enums/OPtextureFormat.h"
#include "./Human/include/Rendering/Enums/OPtextureWrap.h"
#include "./Human/include/Rendering/Enums/OPtextureType.h"
#include "./Core/include/OPtypes.h"

struct OPtextureDesc {
	ui32 width;
	ui32 height;
	OPtextureFormat format;
	OPtextureWrap wrap;
	OPtextureFilter filter;
	OPtextureType textureType;

	OPtextureDesc() {
		width = 0;
		height = 0;
		this->format = OPtextureFormat::RGBA;
		this->wrap = OPtextureWrap::CLAMP;
		this->filter = OPtextureFilter::LINEAR;
		this->textureType = OPtextureType::BYTE;
	}
	OPtextureDesc(ui32 width, ui32 height) {
		this->width = width;
		this->height = height;
		this->format = OPtextureFormat::RGBA;
		this->wrap = OPtextureWrap::CLAMP;
		this->filter = OPtextureFilter::LINEAR;
		this->textureType = OPtextureType::BYTE;
	}
	OPtextureDesc(ui32 width, ui32 height, OPtextureFormat format, OPtextureWrap wrap, OPtextureFilter filter) {
		this->width = width;
		this->height = height;
		this->format = format;
		this->wrap = wrap;
		this->filter = filter;
		this->textureType = OPtextureType::BYTE;
	}
	OPtextureDesc(ui32 width, ui32 height, OPtextureFormat format, OPtextureWrap wrap, OPtextureFilter filter, OPtextureType textureType) {
		this->width = width;
		this->height = height;
		this->format = format;
		this->wrap = wrap;
		this->filter = filter;
		this->textureType = textureType;
	}
};
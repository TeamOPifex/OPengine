#pragma once
#include <stdlib.h>
#include "./Human/Resources/Resource.h"

class Image : public Resource {
public:
	~Image() {
		free(_buffer);
	}
	ui32 Width() { return _width; }
	ui32 Height() { return _height; }
	ui32 LinearSize() { return _linearSize; }
	ui32 MipMapCount() { return _mipMapCount; }
	ui32 Blocksize() { return _blocksize; }
	ui32 FourCC() { return _fourCC; }
	ui8* Buffer() { return _buffer; }
	ui32 Format() { return _format; }
protected:
	ui32 _width;
	ui32 _height;
	ui32 _linearSize;
	ui32 _mipMapCount;
	ui32 _blocksize;
	ui32 _fourCC;
	ui32 _format;
	ui8* _buffer;
	ui32 bufsize;
};

typedef Image* ImagePtr;
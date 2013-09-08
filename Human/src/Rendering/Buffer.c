#include "./Human/include/Rendering/Buffer.h"
#include "./Human/include/Utilities/Errors.h"

#ifdef OPIFEX_OPENGL_ES_2
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#else
	#include <GL/glew.h>
#endif

//-----------------------------------------------------------------------------
// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
OPrenderBuffer OPrenderGenBuffer(ui32 type){
	OPrenderBuffer out = {
		type,
		-1
	};

	glGenBuffers(1, &out.Handle);
	CheckError("OPrenderGenBuffer() - ERROR!");

	return out;
}
//-----------------------------------------------------------------------------
void OPrenderSetBufferData(OPrenderBuffer* buff, ui32 elementSize, ui32 count, const void* data){
	OPrenderBindBuffer(buff);
	buff->ElementSize  = elementSize;
	buff->ElementCount = count;

	CheckError("OPrenderBindBuffer() - ERROR!");
	glBufferData(buff->Type, elementSize * count, data, GL_STATIC_DRAW);
	CheckError("OPrenderSetBufferData() - ERROR!");
}
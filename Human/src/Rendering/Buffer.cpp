#include "./Human/include/Rendering/Buffer.h"
#include "./Human/include/Utilities/Errors.h"

#ifdef OPIFEX_OPENGL_ES_2
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#else
	#include <GL/glew.h>
#endif

//-----------------------------------------------------------------------------
//   _____ _       _           _     
//  / ____| |     | |         | |    
// | |  __| | ___ | |__   __ _| |___ 
// | | |_ | |/ _ \| '_ \ / _` | / __|
// | |__| | | (_) | |_) | (_| | \__ \
//  \_____|_|\___/|_.__/ \__,_|_|___/
OPrenderBuffer* OPRENDER_CURR_VB;
OPrenderBuffer* OPRENDER_CURR_IB;

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
void OPrenderDelBuffer(OPrenderBuffer* buff){
	glDeleteBuffers(1, &buff->Handle);
}
//-----------------------------------------------------------------------------
void OPrenderSetBufferData(OPrenderBuffer* buff, ui32 elementSize, ui32 count, const void* data){
	OPrenderBindBuffer(buff);
	buff->ElementSize  = elementSize;
	buff->ElementCount = count;

	glBufferData(buff->Type, elementSize * count, data, GL_STATIC_DRAW);
}
//-----------------------------------------------------------------------------
void OPrenderBindBuffer(OPrenderBuffer* buffer){
	glBindBuffer(buffer->Type, buffer->Handle);
	if(buffer->Type == OPvertexBuffer){
		OPRENDER_CURR_VB = buffer;
	}
	else{
		OPRENDER_CURR_IB = buffer;
	}
}
//-----------------------------------------------------------------------------
void OPrenderDrawBuffer(ui32 offset){
	glDrawArrays(GL_TRIANGLES, offset, OPRENDER_CURR_VB->ElementCount);
}
//-----------------------------------------------------------------------------
void OPrenderDrawBufferIndexed(ui32 offset){
	glDrawElements(GL_TRIANGLES, OPRENDER_CURR_IB->ElementCount, GL_UNSIGNED_SHORT, (void*)(0 + offset));
}
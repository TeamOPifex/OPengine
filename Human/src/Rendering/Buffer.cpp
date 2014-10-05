#include "./Human/include/Rendering/Buffer.h"
#include "./Human/include/Utilities/Errors.h"
#include "./Human/include/Attributes.h"
#include "./Core/include/Log.h"

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
	OPglError("OPrenderGenBuffer() - ERROR!");

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
	OPglError("OPrenderSetBufferData() - ERROR!");
}

void OPrenderSetBufferSubData(OPrenderBuffer* buff, ui32 elementSize, ui32 offsetCount, ui32 count, const void* data) {
	OPrenderBindBuffer(buff);
	glBufferSubData(buff->Type, offsetCount * elementSize, count * elementSize, data);
}
//-----------------------------------------------------------------------------
void OPrenderBindBuffer(OPrenderBuffer* buffer){
	
	OPglError("OPrenderBindBuffer:Error 0");
	glBindBuffer(buffer->Type, buffer->Handle);
	if(OPglError("OPrenderBindBuffer:Error 1")) {
		OPlog("Buffer Failed to Bind - Type (%d) / Handle (%d)", buffer->Type, buffer->Handle);
	}
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
	OPglError("OPrenderDrawBufferIndexed:Error 0");
	GLenum indType = OPRENDER_CURR_IB->ElementSize == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
	glDrawElements(GL_TRIANGLES, OPRENDER_CURR_IB->ElementCount, indType, (void*)(0 + offset));
	OPglError("OPrenderDrawBufferIndexed:Error 1");
}

//-----------------------------------------------------------------------------
void OPrenderDrawIndexed(ui32 offset, ui32 count){
	GLenum indType = OPRENDER_CURR_IB->ElementSize == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
	glDrawElements(GL_TRIANGLES, count, indType, (void*)(0 + offset));
}

void OPrenderDrawUserArray(void* vertices, ui32 attrs, ui32 offset, ui32 count) {

#ifdef OPIFEX_ANDROID
	OPlog("OPrenderDrawUserArray NOT SUPPORTED yet on Android");
#else
	ui32 off = 0;
	ui32 stride = 0;
	if (attrs & OPATTR_POSITION) stride += 3;
	if (attrs & OPATTR_NORMAL) stride += 3;
	if (attrs & OPATTR_UV) stride += 2;

	if (attrs & OPATTR_POSITION) {
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, stride, &vertices);
		off += 3;
	}

	if (attrs & OPATTR_NORMAL) {
		glDisableClientState(GL_VERTEX_ARRAY);
		glNormalPointer(GL_FLOAT, stride, &vertices + off * sizeof(f32));
		off += 3;
	}

	if (attrs & OPATTR_UV) {
		glDisableClientState(GL_VERTEX_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, stride, &vertices + off * sizeof(f32));
		off += 2;
	}

	glDrawArrays(GL_TRIANGLES, offset, count);

	if (attrs & OPATTR_POSITION) {
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	if (attrs & OPATTR_NORMAL) {
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	if (attrs & OPATTR_UV) {
		glDisableClientState(GL_VERTEX_ARRAY);
	}
#endif
}
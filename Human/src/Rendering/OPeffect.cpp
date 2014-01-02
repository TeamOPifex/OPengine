#include "./Human/include/Rendering/OPeffect.h"

#ifdef OPIFEX_OPENGL_ES_2
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#include <GL/glfw.h>
//#include <glm/glm.hpp>
#endif


OPeffect* OPRENDER_CURR_EFFECT = NULL;
//-----------------------------------------------------------------------------
// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
// Shader creation
OPint OPrenderLoadVertexShader(const OPchar* filename, OPshader** shader){
	OPshader vertex = -1;

	OPLog("OPrenderLoadVertexShader: %s", filename);
	OPstream* source = OPreadFile(filename);
	
	CheckError("GLShader::Error 0");
	vertex = glCreateShader(OPvertexShader);
	CheckError("GLShader::Error 1");
	if(vertex){
		OPchar* src = (OPchar*)source->Data;
		
		glShaderSource(vertex, 1, (const OPchar**)&src, 0);
		CheckError("GLShader::Error 2");
		glCompileShader(vertex);
		CheckError("GLShader::Error 3");
		GLint compiled = 0;
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &compiled);
		CheckError("GLShader::Error 4");
		if(!compiled){

			char msg[4096];
			i32 length = 0;
			glGetShaderInfoLog(vertex, 4096, &length, msg);
			OPLog("GLShader::Failed to compile Vertex Shader::%s", msg);

			CheckError("GLShader::Error 5");
			glDeleteShader(vertex);
			OPstreamDestroy(source); // clean up stream
			return 0;
		}
	}
	else{
		OPstreamDestroy(source); // clean up stream
		return 0;
	}
	OPstreamDestroy(source); // clean up stream
	
	// if we made it this far, everything is a-ok
	*shader = (OPshader*)OPalloc(sizeof(OPshader));
	**shader = vertex; // copy the shader handle

	CheckError("GLShader::Error 6");

	return 1;
}
//-----------------------------------------------------------------------------
OPint OPrenderLoadFragmentShader(const OPchar* filename, OPshader** shader){
	OPshader frag = -1;
	OPstream* source = OPreadFile(filename);

	frag = glCreateShader(OPfragmentShader);

	if(frag){
		OPchar* src = (OPchar*)source->Data;
		glShaderSource(frag, 1, (const OPchar**)&src, 0);
		glCompileShader(frag);

		GLint compiled = 0;
		glGetShaderiv(frag, GL_COMPILE_STATUS, &compiled);
		if(!compiled){

			char msg[4096];
			i32 length = 0;
			glGetShaderInfoLog(frag, 4096, &length, msg);

			OPLog("GLShader::Failed to compile Fragment Shader::%s", msg);

			glDeleteShader(frag);
			OPstreamDestroy(source); // clean up stream
			return 0;
		}
	}
	else{
		OPstreamDestroy(source); // clean up stream
		return 0;
	}
	OPstreamDestroy(source); // clean up stream
	
	// if we made it this far, everything is a-ok
	*shader = (OPshader*)OPalloc(sizeof(OPshader));
	**shader = frag; // copy the shader handle

	return 1;
}
//-----------------------------------------------------------------------------
// shader destruction
OPint OPrenderUnloadShader(OPshader* shader){
	glDeleteShader(*shader); // free up the gl shader
	OPfree(shader); // free up the integer
	return 1;
}
//-----------------------------------------------------------------------------
// effect creation
OPeffect OPrenderCreateEffect(

	CheckError("OPrenderCreateEffect:Error 0");
	OPshader vert,
	OPshader frag,
	OPshaderAttribute* Attributes,
	OPint AttribCount,
	const OPchar* Name){

	OPint nameLen = strlen(Name) + 1;
	OPeffect effect = {
		vert,
		frag,
		-1,
		 0
	};

	// copy the name into the struct
	OPmemcpy(
		effect.Name, 
		Name, 
		nameLen > OP_EFFECT_NAME_LEN ? OP_EFFECT_NAME_LEN : nameLen
	); effect.Name[OP_EFFECT_NAME_LEN - 1] = '\0';

	effect.Parameters = OPhashMapCreate(32);
	effect.Attributes = OPlistCreate(AttribCount, sizeof(OPshaderAttribute));

	effect.ProgramHandle = glCreateProgram();

	glAttachShader(effect.ProgramHandle, vert);
	glAttachShader(effect.ProgramHandle, frag);
	glLinkProgram(effect.ProgramHandle);

	OPint status;
	glGetProgramiv(effect.ProgramHandle, GL_LINK_STATUS, &status);
	glUseProgram(effect.ProgramHandle);

	if(status == GL_FALSE) {
		OPLog("FAILED to link Shader Program");
	}
	
	OPLog("Shader Program Status: %d", status);
	CheckError("OPrenderCreateEffect:Error 7");

	// create, and copy attributes into list
	for(OPint i = 0; i < AttribCount; i++){
		OPshaderAttribute attr = {
			NULL,
			Attributes[i].Type,
			Attributes[i].Elements,
			(void*)effect.Stride
		};

		attr.Name = (OPchar*)glGetAttribLocation(
			effect.ProgramHandle,
			Attributes[i].Name
		);
		if(CheckError("OPrenderCreateEffect:Error 7.5 - Attrib Could not be found.") > 0) {
			OPLog("Handle: %d, Attribute: %s", (OPint)attr.Name, Attributes[i].Name);
		}
		else{
			OPLog("OK!!! Handle: %d, Attribute: %s", (OPint)attr.Name, Attributes[i].Name);
		}

		// TODO add more
		switch(Attributes[i].Type){
			case GL_FLOAT:
				effect.Stride += (4 * Attributes[i].Elements);
				break;
		}

		OPlistPush(effect.Attributes, (ui8*)&attr);
	}
	
	return effect;
}
//-----------------------------------------------------------------------------
OPint OPrenderLoadEffect  (const OPchar* filename, OPeffect** effect){
	return 1;
}
//-----------------------------------------------------------------------------
// effect destruction
OPint OPrenderUnloadEffect(OPeffect* effect){
	OPhashMapDestroy(effect->Parameters);
	OPlistDestroy(effect->Attributes);
	glDeleteProgram(effect->ProgramHandle);
	
	return 1;
}

// effect managment
OPint OPrenderBindEffect(OPeffect* effect){
	// disable attributes of the last effect
	if(OPRENDER_CURR_EFFECT){
		OPint attrCount = OPlistSize(OPRENDER_CURR_EFFECT->Attributes);
		for(;attrCount--;){
			OPshaderAttribute* attr = (OPshaderAttribute*)OPlistGet(OPRENDER_CURR_EFFECT->Attributes, attrCount);
			glDisableVertexAttribArray((ui32)attr->Name);
			if(CheckError("OPrenderBindEffect:Error ")) {
				OPLog("Effect %s: Failed to disable attrib %u", OPRENDER_CURR_EFFECT->Name, (ui32)attr->Name);
			}
		}
	}

	OPRENDER_CURR_EFFECT = effect;

	glUseProgram(OPRENDER_CURR_EFFECT->ProgramHandle);

	// enable attributes of the new effect
	OPint attrCount = OPlistSize(OPRENDER_CURR_EFFECT->Attributes);
	for(;attrCount--;){
		OPshaderAttribute* attr = (OPshaderAttribute*)OPlistGet(OPRENDER_CURR_EFFECT->Attributes, attrCount);
		
		glEnableVertexAttribArray((ui32)attr->Name);
		if(CheckError("OPrenderBindEffect:Error ")) {
			OPLog("Failed to enable attrib %u", (ui32)attr->Name);
		}
		glVertexAttribPointer(
			(ui32)attr->Name,
			attr->Elements,
			attr->Type,
			GL_FALSE,
			effect->Stride,
			attr->Offset
		);
		if(CheckError("OPrenderBindEffect:Error ")) {
			OPLog("Effect %s: Failed to set attrib ptr %u", effect->Name, (ui32)attr->Name);
		}
	}

	return 1;
}

ui32 OPrenderGetParam(const OPchar* parameterName){
	if(OPhashMapExists(OPRENDER_CURR_EFFECT->Parameters, parameterName)){
		ui32* loc;
		OPhashMapGet(
			OPRENDER_CURR_EFFECT->Parameters,
			parameterName,
			(void**)&loc
		);
		return *loc;
	}
	else{
		ui32 loc = glGetUniformLocation(
			OPRENDER_CURR_EFFECT->ProgramHandle,
			parameterName
		);

		ui32* locPtr = (ui32*)OPalloc(sizeof(ui32));
		*locPtr = loc;
		OPhashMapPut(OPRENDER_CURR_EFFECT->Parameters, parameterName, &locPtr);
		return loc;
	}
}

void OPrenderUseTexture(const OPchar* param, ui32 texture, ui32 slot){
	GLuint loc = glGetUniformLocation(
		OPRENDER_CURR_EFFECT->ProgramHandle,
		param
	);
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(loc, slot);
}

#include "./Human/include/Rendering/Effect.h"
#include "./Human/include/Rendering/OpenGL.h"
#include "./Core/include/Assert.h"
#include "./Data/include/Vector.h"
#include "./Data/include/ContentManager.h"

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

	OPstream* source = OPreadFile(filename);
	
	OPglError("GLShader::Error 0");
	vertex = glCreateShader(OPvertexShader);
	OPglError("GLShader::Error 1");
	if(vertex){
		OPchar* src = (OPchar*)source->Data;
		
		glShaderSource(vertex, 1, (const OPchar**)&src, 0);
		OPglError("GLShader::Error 2");
		glCompileShader(vertex);
		OPglError("GLShader::Error 3");
		GLint compiled = 0;
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &compiled);
		OPglError("GLShader::Error 4");

		if(!compiled){
			char msg[4096];
			i32 length = 0;
			glGetShaderInfoLog(vertex, 4096, &length, msg);
			OPlog("GLShader::Failed to compile Vertex Shader::%s", msg);

			OPglError("GLShader::Error 5");
			glDeleteShader(vertex);
			OPstreamDestroy(source); // clean up stream
			return -1;
		}
	}
	else{
		OPstreamDestroy(source); // clean up stream
		return -1;
	}
	OPstreamDestroy(source); // clean up stream
	
	// if we made it this far, everything is a-ok
	*shader = (OPshader*)OPalloc(sizeof(OPshader));
	**shader = vertex; // copy the shader handle

	OPglError("GLShader::Error 6");

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

			OPlog("GLShader::Failed to compile Fragment Shader::%s", msg);

			glDeleteShader(frag);
			OPstreamDestroy(source); // clean up stream
			return -1;
		}
	}
	else{
		OPstreamDestroy(source); // clean up stream
		return -1;
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

OPeffect createEffect(OPshader vert,
	OPshader frag,
	OPshaderAttribute* Attributes,
	OPint AttribCount,
	const OPchar* Name,
	ui32 stride) {

	OPglError("OPrenderCreateEffect:Error 0");

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

	i32 status;
	glGetProgramiv(effect.ProgramHandle, GL_LINK_STATUS, &status);
	glUseProgram(effect.ProgramHandle);
	ASSERT(status == GL_TRUE, "Failed to Link Shader Program");

	if (status == GL_FALSE) {
		OPlog("FAILED to link Shader Program: %s", Name);
	}
	else {
		OPglError("OPrenderCreateEffect:Error 7");

		i32 result;
		ui32 nameLength;
		// create, and copy attributes into list
		for (OPint i = 0; i < AttribCount; i++){
			OPshaderAttribute attr = {
				NULL,
				Attributes[i].Type,
				Attributes[i].Elements,
				(void*)effect.Stride,
				0
			};

			// TODO add more
			switch (Attributes[i].Type){
			case GL_FLOAT:
				effect.Stride += (4 * Attributes[i].Elements);
				break;
			}
			nameLength = strlen(Attributes[i].Name);
			attr.Name = (OPchar*)OPalloc(sizeof(OPchar)* nameLength + 1);
			OPmemcpy((void*)attr.Name, (void*)Attributes[i].Name, nameLength + 1);

			result = glGetAttribLocation(
				effect.ProgramHandle,
				Attributes[i].Name
				);
			attr.Handle = (OPuint)result;

			if (result < 0) {
				OPlog("FAILED to find attribute: '%s' in effect '%s'", Attributes[i].Name, effect.Name);
			}
			else {
				OPlistPush(effect.Attributes, (ui8*)&attr);
			}
		}

		effect.Stride = stride;
	}
	
	return effect;
}

//-----------------------------------------------------------------------------
// effect creation
OPeffect OPrenderCreateEffectStride(
	OPshader vert,
	OPshader frag,
	OPshaderAttribute* Attributes,
	OPint AttribCount,
	const OPchar* Name,
	ui32 stride) {
	return createEffect(vert, frag, Attributes, AttribCount, Name, stride);
}
//-----------------------------------------------------------------------------
// effect creation
OPeffect OPrenderCreateEffect(
	OPshader vert,
	OPshader frag,
	OPshaderAttribute* Attributes,
	OPint AttribCount,
	const OPchar* Name){
	
	ui32 stride = 0;

	for (OPint i = 0; i < AttribCount; i++){
		// TODO add more
		switch (Attributes[i].Type){
			case GL_FLOAT:
				stride += (4 * Attributes[i].Elements);
				break;
		}
	}
	return createEffect(vert, frag, Attributes, AttribCount, Name, stride);
}
//-----------------------------------------------------------------------------
OPint OPrenderLoadEffect  (const OPchar* filename, OPeffect** effect){
	return 1;
}
//-----------------------------------------------------------------------------
// effect destruction
OPint OPrenderUnloadEffect(OPeffect* effect){
	if (OPRENDER_CURR_EFFECT == effect) {
		OPrenderBindEffect(NULL);
	}
	OPhashMapDestroy(effect->Parameters);
	OPlistDestroy(effect->Attributes);
	glDeleteProgram(effect->ProgramHandle);
	
	return 1;
}

// effect managment
OPint OPrenderBindEffect(OPeffect* effect){
	OPglError("OPrenderBindEffect:Clear Errors");
	// disable attributes of the last effect
	if(OPRENDER_CURR_EFFECT){
		OPint attrCount = OPlistSize(OPRENDER_CURR_EFFECT->Attributes);
		for(;attrCount--;){
			OPshaderAttribute* attr = (OPshaderAttribute*)OPlistGet(OPRENDER_CURR_EFFECT->Attributes, attrCount);
			glDisableVertexAttribArray((uintptr_t)attr->Handle);
			if (OPglError("OPrenderBindEffect:Error ")) {
				OPlog("Effect %s: Failed to disable attrib %s", OPRENDER_CURR_EFFECT->Name, attr->Name);
			}
		}
	}

	OPRENDER_CURR_EFFECT = effect;

	if (OPRENDER_CURR_EFFECT == NULL) return 1;

	glUseProgram(OPRENDER_CURR_EFFECT->ProgramHandle);
	if (OPglError("OPrenderBindEffect:Failed to use Program")) {
		OPlog("For Shader: %s", OPRENDER_CURR_EFFECT->Name);
	}

	// enable attributes of the new effect
	OPint attrCount = OPlistSize(OPRENDER_CURR_EFFECT->Attributes);
	for(;attrCount--;){
		OPshaderAttribute* attr = (OPshaderAttribute*)OPlistGet(OPRENDER_CURR_EFFECT->Attributes, attrCount);
		glEnableVertexAttribArray((uintptr_t)attr->Handle);
		if (OPglError("OPrenderBindEffect:Error ")) {
			OPlog("Failed to enable attrib %s", attr->Name);
		}
		glVertexAttribPointer(
			(uintptr_t)attr->Handle,
			attr->Elements,
			attr->Type,
			GL_FALSE,
			effect->Stride,
			attr->Offset
			);
		if (OPglError("OPrenderBindEffect:Error ")) {
			OPlog("Effect %s: Failed to set attrib ptr %s", effect->Name, attr->Name);
		}
	}

	OPglError("OPrenderBindEffect:Errors Occured");

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

//-----------------------------------------------------------------------------
// effect creation
OPeffect OPrenderGenEffect(
	const OPchar* vert,
	const OPchar* frag,
	ui32 attrs,
	const OPchar* Name,
	ui32 stride) {

	OPlog("Building Effect");

	OPvector* vector = OPvectorCreate(sizeof(OPshaderAttribute));


	if (attrs & OPATTR_POSITION) {
		OPshaderAttribute attr = { "aPosition", GL_FLOAT, 3 };
		OPvectorPush(vector, (ui8*)&attr);
	}

	if (attrs & OPATTR_NORMAL) {
		OPshaderAttribute attr = { "aNormal", GL_FLOAT, 3 };
		OPvectorPush(vector, (ui8*)&attr);
	}

	if (attrs & OPATTR_TANGENT) {
		OPshaderAttribute attr = { "aTangent", GL_FLOAT, 3 };
		OPvectorPush(vector, (ui8*)&attr);
	}

	if (attrs & OPATTR_UV) {
		OPshaderAttribute attr = { "aUV", GL_FLOAT, 2 };
		OPvectorPush(vector, (ui8*)&attr);
	}

	ui32 AttribCount = vector->_size;
	OPshaderAttribute* Attributes = (OPshaderAttribute*)OPalloc(sizeof(OPshaderAttribute)* vector->_size);
	OPmemcpy(Attributes, vector->items, sizeof(OPshaderAttribute)* vector->_size);
	OPvectorDestroy(vector);


	OPlog("Finding Effect Stride");

	if (stride == 0){
		for (OPint i = 0; i < AttribCount; i++){
			// TODO add more
			switch (Attributes[i].Type){
			case GL_FLOAT:
				stride += (4 * Attributes[i].Elements);
				break;
			}
		}
	}

	OPlog("Loading Vert for Effect");

	if (!OPcmanIsLoaded(vert)) OPcmanLoad(vert);
	OPshader* vertShader = (OPshader*)OPcmanGet(vert);

	OPlog("Loading Frag for Effect");

	if (!OPcmanIsLoaded(frag)) OPcmanLoad(frag);
	OPshader* fragShader = (OPshader*)OPcmanGet(frag);

	OPlog("Create the Effect");

	return createEffect(*vertShader, *fragShader, Attributes, AttribCount, Name, stride);
}
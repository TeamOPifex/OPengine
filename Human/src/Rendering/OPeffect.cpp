#include <include/Rendering/OPrender.h>
#include "./Human/include/Rendering/OPeffect.h"
#include "./Human/include/Rendering/OpenGL.h"
#include "./Human/include/Rendering/OPmesh.h"
#include "./Core/include/Assert.h"
#include "./Data/include/OPvector.h"
#include "./Data/include/OPcman.h"
#include "./Data/include/OPstring.h"

OPeffect* OPEFFECT_ACTIVE = NULL;
OPmesh* OPEFFECT_BOUND_MESH = NULL;

OPeffect createEffect(OPshader vert,
	OPshader frag,
	OPshaderAttribute* Attributes,
	OPint AttribCount,
	const OPchar* Name,
	ui32 stride) {

	OPglError("OPeffectCreate:Error 0: %d");

	OPint nameLen = strlen(Name) + 1;
	OPeffect effect = {
		vert,
		frag,
		0,
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
		OPglError("OPeffectCreate:Error 7");

		i32 result;
		ui32 nameLength;
		// create, and copy attributes into list
		for (OPint i = 0; i < AttribCount; i++){
			OPshaderAttribute attr = {
				NULL,
				Attributes[i].Type,
				Attributes[i].Elements,
				effect.Stride,
				0
			};

			// TODO add more
			switch (Attributes[i].Type){
			case GL_FLOAT:
				effect.Stride += (4 * Attributes[i].Elements);
				break;
			}
			attr.Name = OPstringCopy(Attributes[i].Name);

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
OPeffect OPeffectCreate(
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
OPeffect OPeffectCreate(
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
// effect destruction
OPint OPeffectUnload(OPeffect* effect){
	if (OPEFFECT_ACTIVE == effect) {
		OPeffectBind(NULL);
	}
	OPhashMapDestroy(effect->Parameters);
	OPlistDestroy(effect->Attributes);
	glDeleteProgram(effect->ProgramHandle);
	
	return 1;
}

// effect managment
OPint OPeffectBind(OPeffect* effect){
	OPglError("OPeffectBind:Clear Errors");

	// if (OPEFFECT_ACTIVE == effect && OPEFFECT_BOUND_MESH == OPMESH_ACTIVE_PTR) {
	// 	//OPlog("Already bound, redoing it anyway");
	// 	//return 1;
	// }


	// disable attributes of the last effect
	if (OPEFFECT_ACTIVE){
		OPint attrCount = OPlistSize(OPEFFECT_ACTIVE->Attributes);
		for (; attrCount--;){
			OPshaderAttribute* attr = (OPshaderAttribute*)OPlistGet(OPEFFECT_ACTIVE->Attributes, attrCount);
			glDisableVertexAttribArray((uintptr_t)attr->Handle);
			if (OPglError("OPeffectBind:Error ")) {
				OPlog("Effect %s: Failed to disable attrib %s", OPEFFECT_ACTIVE->Name, attr->Name);
			}
		}
	}

	OPEFFECT_ACTIVE = effect;
	OPEFFECT_BOUND_MESH = OPMESH_ACTIVE;

	if (OPEFFECT_ACTIVE == NULL) return 1;

	glUseProgram(OPEFFECT_ACTIVE->ProgramHandle);

	if (OPglError("OPeffectBind:Failed to use Program")) {
		OPlog("For Shader: %s", OPEFFECT_ACTIVE->Name);
		return -1;
	}

	// enable attributes of the new effect
	OPint attrCount = OPlistSize(OPEFFECT_ACTIVE->Attributes);
	for (; attrCount--;){
		OPshaderAttribute* attr = (OPshaderAttribute*)OPlistGet(OPEFFECT_ACTIVE->Attributes, attrCount);

		glEnableVertexAttribArray((uintptr_t)attr->Handle);
		if (OPglError("OPeffectBind:Error ")) {
			OPlog("Failed to enable attrib %s", attr->Name);
		}

		glVertexAttribPointer(
			(uintptr_t)attr->Handle,
			attr->Elements,
			attr->Type,
			GL_FALSE,
			effect->Stride,
			(void*)attr->Offset
			);
		if (OPglError("OPeffectBind:Error ")) {
			OPlog("Effect %s: Failed to set attrib ptr %s", effect->Name, attr->Name);
		}
//		else {
//			OPlog("Set %s", attr->Name);
//		}
	}

	OPglError("OPeffectBind:Errors Occured");

	return 1;
	
}

ui32 OPeffectGetParam(const OPchar* parameterName){
	if(OPhashMapExists(OPEFFECT_ACTIVE->Parameters, parameterName)){
		ui32* loc;
		OPhashMapGet(
			OPEFFECT_ACTIVE->Parameters,
			parameterName,
			(void**)&loc
		);
		return *loc;
	}
	else{
		ui32 loc = (ui32)glGetUniformLocation(
			OPEFFECT_ACTIVE->ProgramHandle,
			parameterName
		);

		ui32* locPtr = (ui32*)OPalloc(sizeof(ui32));
		*locPtr = loc;
		OPhashMapPut(OPEFFECT_ACTIVE->Parameters, parameterName, &locPtr);
		return loc;
	}
}

//-----------------------------------------------------------------------------
// effect creation
OPeffect OPeffectGen(
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

	if (attrs & OPATTR_BONES) {
		OPshaderAttribute attr1 = { "aBones", GL_FLOAT, 4 };
		OPvectorPush(vector, (ui8*)&attr1);
		OPshaderAttribute attr2 = { "aWeights", GL_FLOAT, 4 };
		OPvectorPush(vector, (ui8*)&attr2);
	}

	if (attrs & OPATTR_COLOR) {
		OPshaderAttribute attr = { "aColor", GL_FLOAT, 3 };
		OPvectorPush(vector, (ui8*)&attr);
	}

	if (attrs & OPATTR_COLOR4) {
		OPshaderAttribute attr = { "aColor", GL_FLOAT, 4 };
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
				stride += (sizeof(f32) * Attributes[i].Elements);
				break;
			}
		}
	}

	OPlog("Loading Vert for Effect: %s", vert);

	if (!OPcmanIsLoaded(vert)) {
		OPlog("Wasn't already loaded. Loading it.");
		OPcmanLoad(vert);
	} else {
		OPlog("Already loaded.c");
	}
	OPshader* vertShader = (OPshader*)OPcmanGet(vert);

	OPlog("Loading Frag for Effect: %s", frag);

	if (!OPcmanIsLoaded(frag)) OPcmanLoad(frag);
	OPshader* fragShader = (OPshader*)OPcmanGet(frag);

	OPlog("Create the Effect");

	return createEffect(*vertShader, *fragShader, Attributes, AttribCount, Name, stride);
}
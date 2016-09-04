#pragma once

struct OPshader;
struct OPshaderAttribute;

typedef struct OPshader OPshader;
typedef struct OPshaderAttribute OPshaderAttribute;

#include "./Human/include/Rendering/Enums/OPshaderType.h"
#include "./Data/include/OPstream.h"
#include "./Data/include/OPcman.h"

struct OPshader {
	void* internalPtr;
	OPshaderType shaderType;
};

struct OPshaderAttribute {
	OPchar* Name;
	OPshaderElementType Type;
	ui32 Elements;
	OPuint Offset;
	OPint Location;

	void* internalPtr;
};

OPint OPshaderLoadVertex(OPstream* str, OPshader** shader);
OPint OPshaderLoadFragment(OPstream* str, OPshader** shader);
OPint OPshaderReloadVertex(OPstream* str, OPshader** shader);
OPint OPshaderReloadFragment(OPstream* str, OPshader** shader);
OPint OPshaderUnload(OPshader* shader);

extern OPassetLoader OPASSETLOADER_VERT;
extern OPassetLoader OPASSETLOADER_FRAG;
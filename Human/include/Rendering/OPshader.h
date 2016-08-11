#ifndef OPENGINE_HUMAN_RENDERER_SHADER
#define OPENGINE_HUMAN_RENDERER_SHADER

#include "./Core/include/OPtypes.h"
#include "./Data/include/OPstream.h"

//-----------------------------------------------------------------------------
//  _____                     _____                  _____  _               _   _
// |  __ \                   |  __ \                |  __ \(_)             | | (_)
// | |__) _ __ ___   ______  | |__) _ __ ___   ___  | |  | |_ _ __ ___  ___| |_ ___   _____ ___
// |  ___| '__/ _ \ |______| |  ___| '__/ _ \ / __| | |  | | | '__/ _ \/ __| __| \ \ / / _ / __|
// | |   | | |  __/          | |   | | | (_) | (__  | |__| | | | |  __| (__| |_| |\ V |  __\__ \
// |_|   |_|  \___|          |_|   |_|  \___/ \___| |_____/|_|_|  \___|\___|\__|_| \_/ \___|___/

#include "./Human/include/Rendering/Enums/OPshaderType.h"

struct OPshader {
	void* internalPtr;
	OPshaderType shaderType;
};


//-----------------------------------------------------------------------------
//   _____ _                   _
//  / ____| |                 | |
// | (___ | |_ _ __ _   _  ___| |_ ___
//  \___ \| __| '__| | | |/ __| __/ __|
//  ____) | |_| |  | |_| | (__| |_\__ \
// |_____/ \__|_|   \__,_|\___|\__|___/

struct OPshaderAttribute {
	OPchar* Name;
	OPshaderElementType Type;
	ui32 Elements;
	OPuint Offset;
	OPint Location;

	void* internalPtr;
};


//-----------------------------------------------------------------------------
// ______                _   _
//|  ____|              | | (_)
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/

OPint OPshaderLoadVertex(OPstream* str, OPshader** shader);
OPint OPshaderLoadFragment(OPstream* str, OPshader** shader);
//OPint OPshaderLoadVertexDX11(OPstream* source, OPshader** shader);
//OPint OPshaderLoadFragmentDX11(OPstream* source, OPshader** shader);
OPint OPshaderUnload(OPshader* shader);


#endif

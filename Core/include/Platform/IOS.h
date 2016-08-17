#pragma once

#include "./Core/include/OPtypes.h"

#ifdef OPIFEX_IOS
	void OPstartRender();
	OPint OPstartUpdate();
	void OPstart(int argc, char** args);

	#define OP_MAIN int _OP_WRAPPED_MAIN(int argc, char** args) {
	#define OP_MAIN_START OPstart(argc, args);
	#define OP_MAIN_START_STEPPED OPstartStepped(argc, args);
	#define OP_MAIN_END OPend();
	#define OP_MAIN_SUCCESS return 0;
#endif

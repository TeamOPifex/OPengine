#include "./Scripting/include/OPloaderOPS.h"
#include "./Data/include/OPfile.h"
#include "./Core/include/DynamicMemory.h"
#include "./Data/include/OPcman.h"

void OPscriptAddLoader() {

	OPassetLoader loaderOPS = {
		".ops",
		"Scripts/",
		sizeof(OPscript),
		(OPint(*)(const OPchar*, void**))OPscriptLoad,
		(OPint(*)(void*))OPscriptUnload,
		NULL
	};
	OPcmanAddLoader(&loaderOPS);

	OPassetLoader loaderJS = {
		".js",
		"Scripts/",
		sizeof(OPscript),
		(OPint(*)(const OPchar*, void**))OPscriptLoad,
		(OPint(*)(void*))OPscriptUnload,
		NULL
	};
	OPcmanAddLoader(&loaderJS);
	
}

OPint OPscriptLoad(const OPchar* filename, OPscript** script) {
	*script = (OPscript*)OPalloc(sizeof(OPscript));
	OPstream* str = OPreadFile(filename);
	(*script)->data = (OPchar*)OPalloc(str->Length);
	OPmemcpy((*script)->data, str->Data, str->Length);
	return 1;
}

OPint OPscriptUnload(OPscript* script) {
	OPfree(script->data);
	OPfree(script);
	return 1;
}
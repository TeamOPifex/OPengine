#include "./Scripting/include/OPloaderOPS.h"
#include "./Data/include/OPfile.h"
#include "./Core/include/OPmemory.h"
#include "./Data/include/OPcman.h"
#include "./Core/include/OPlog.h"

void OPscriptAddLoader() {

	OPassetLoader loaderOPS = {
		".ops",
		"Scripts/",
		sizeof(OPscript),
		(OPint(*)(const OPchar*, void**))OPscriptLoad,
		(OPint(*)(void*))OPscriptUnload,
		(OPint(*)(const OPchar*, void**))OPscriptReload
	};
	OPcmanAddLoader(&loaderOPS);

	OPassetLoader loaderJS = {
		".js",
		"Scripts/",
		sizeof(OPscript),
		(OPint(*)(const OPchar*, void**))OPscriptLoad,
		(OPint(*)(void*))OPscriptUnload,
		(OPint(*)(const OPchar*, void**))OPscriptReload
	};
	OPcmanAddLoader(&loaderJS);
	
}

OPint OPscriptLoad(const OPchar* filename, OPscript** script) {
	*script = (OPscript*)OPalloc(sizeof(OPscript));
	OPstream* str = OPreadFile(filename);
	(*script)->data = (OPchar*)OPalloc(str->Length);
	OPmemcpy((*script)->data, str->Data, str->Length);

#ifdef _DEBUG
	(*script)->changed = 0;
#endif

	return 1;
}

OPint OPscriptReload(const OPchar* filename, OPscript** script) {
	OPlog("Reload script");
	OPscript* resultScript;
	OPscript* tex = (OPscript*)(*script);
	OPint result = OPscriptLoad(filename, &resultScript);
	if (result) {
		OPmemcpy(*script, resultScript, sizeof(OPscript));
#ifdef _DEBUG
		(*script)->changed = 1;
#endif
		OPfree(resultScript);
	}
	return result;
}

OPint OPscriptUnload(OPscript* script) {
	OPfree(script->data);
	OPfree(script);
	return 1;
}
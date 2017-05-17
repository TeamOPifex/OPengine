#include "./Scripting/include/OPloaderOPS.h"
#include "./Data/include/OPfile.h"
#include "./Data/include/OPcman.h"
#include "./Data/include/OPstring.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPdebug.h"

void OPscriptAddLoader() {

	OPassetLoader loaderOPS = {
		".ops",
		"Scripts/",
		sizeof(OPscript),
		(OPint(*)(OPstream*, void**))OPscriptLoad,
		(OPint(*)(void*))OPscriptUnload,
		(OPint(*)(OPstream*, void**))OPscriptReload
	};
	OPCMAN.AddLoader(&loaderOPS);

	OPassetLoader loaderJS = {
		".js",
		"Scripts/",
		sizeof(OPscript),
		(OPint(*)(OPstream*, void**))OPscriptLoad,
		(OPint(*)(void*))OPscriptUnload,
		(OPint(*)(OPstream*, void**))OPscriptReload
	};
	OPCMAN.AddLoader(&loaderJS);

}

OPint OPscriptLoad(OPstream* str, OPscript** script) {
	*script = (OPscript*)OPalloc(sizeof(OPscript));
	//OPlog("allocated script");
	(*script)->data = (OPchar*)OPalloc(str->Length);
	//OPlog("allocated data");

	OPmemcpy((*script)->data, str->Data, str->Length);
	//OPlog("copied memory");
	//OPlog("mem %s", str->Source);
	(*script)->filename = OPstringCopy(str->Source);
	//OPlog("copied filename");

#ifdef _DEBUG
	(*script)->changed = 0;
#endif

	return 1;
}

OPint OPscriptReload(OPstream* str, OPscript** script) {
//	OPlog("Reload script");
//	OPscript* resultScript;
//	OPscript* tex = (OPscript*)(*script);
//	OPint result = OPscriptLoad(str, &resultScript);
//	if (result) {
//		OPmemcpy(*script, resultScript, sizeof(OPscript));
//#ifdef _DEBUG
//		(*script)->changed = 1;
//#endif
//		OPfree(resultScript);
//	}
//	return result;
	return 1;
}

OPint OPscriptUnload(OPscript* script) {
	OPfree(script->data);
	OPfree(script->filename);
	OPfree(script);
	return 1;
}

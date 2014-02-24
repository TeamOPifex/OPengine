#include "./Human/include/Utilities/LoaderOPS.h"
#include "./Data/include/File.h"
#include "./Core/include/DynamicMemory.h"

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
#ifndef OPENGINE_DATA_STRING
#define OPENGINE_DATA_STRING

#include "./Core/include/OPtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

	OPint OPstringEquals(const OPchar* str, const OPchar* cmp);
	OPint OPstringStartsWith(OPchar* str, const OPchar* cmp);
	OPint OPstringEndsWith(OPchar* str, const OPchar* cmp);
	void OPstringRemoveFromStart(OPchar* str, i32 size);
	OPchar* OPstringGetNonConstant(const OPchar* str);
	OPchar* OPstringCreateMerged(const OPchar* str, const OPchar* add);
	OPchar* OPstringCopy(const OPchar* str);
	OPchar* OPstringSub(const OPchar* str, OPint start, OPint end);
	OPint OPstringContains(OPchar* str, const OPchar* cmp);
	void OPstringToLower(OPchar* str);

#ifdef __cplusplus
}
#endif

#endif
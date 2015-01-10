#ifndef OPENGINE_DATA_STRING
#define OPENGINE_DATA_STRING

#include "./Core/include/OPtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

	EXPORT_METHOD OPint OPstringEquals(const OPchar* str, const OPchar* cmp);
	EXPORT_METHOD OPint OPstringStartsWith(OPchar* str, const OPchar* cmp, i32 size);
	EXPORT_METHOD void OPstringRemoveFromStart(OPchar* str, i32 size);
	EXPORT_METHOD OPchar* OPstringGetNonConstant(const OPchar* str);
	EXPORT_METHOD OPchar* OPstringCreateMerged(const OPchar* str, const OPchar* add);
	EXPORT_METHOD OPchar* OPstringCopy(const OPchar* str);
	EXPORT_METHOD OPint OPstringContains(OPchar* str, const OPchar* cmp);
	EXPORT_METHOD void OPstringToLower(OPchar* str);

#ifdef __cplusplus
}
#endif

#endif
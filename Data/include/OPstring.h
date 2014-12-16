#ifndef OPENGINE_DATA_STRING
#define OPENGINE_DATA_STRING

#include "./Core/include/Types.h"

#ifdef __cplusplus
extern "C" {
#endif

	OPint OPstringEquals(const OPchar* str, const OPchar* cmp);
	OPint OPstringStartsWith(OPchar* str, const OPchar* cmp, i32 size);
	void OPstringRemoveFromStart(OPchar* str, i32 size);
	OPchar* OPstringGetNonConstant(const OPchar* str);
	OPchar* OPstringCreateMerged(const OPchar* str, const OPchar* add);
	OPchar* OPstringCreateCopy(const OPchar* str);

#ifdef __cplusplus
}
#endif

#endif
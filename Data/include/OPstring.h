#pragma once

#include "./Core/include/OPtypes.h"

OPint OPstringEquals(const OPchar* str, const OPchar* cmp);
OPint OPstringStartsWith(OPchar* str, const OPchar* cmp);
OPint OPstringEndsWith(OPchar* str, const OPchar* cmp);
void OPstringRemoveFromStart(OPchar* str, i32 size);
OPchar* OPstringGetNonConstant(const OPchar* str);
OPchar* OPstringCreateMerged(const OPchar* str, const OPchar* add);
OPchar* OPstringCopy(const OPchar* str);
OPchar* OPstringSub(const OPchar* str, ui32 start, ui32 end);
OPint OPstringContains(OPchar* str, const OPchar* cmp);
void OPstringToLower(OPchar* str);
void OPstringScan(OPchar* destination, const OPchar* format, ...);
i32 OPstringCount(OPchar* str, OPchar lookFor);
i32 OPstringFirst(OPchar* str, OPchar lookFor);
i32 OPstringSplit(const OPchar* str, OPchar split, OPchar*** output);
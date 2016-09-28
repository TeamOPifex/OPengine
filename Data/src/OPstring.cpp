#include "./Data/include/OPstring.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPlog.h"
#include <ctype.h>

OPint OPstringEquals(const OPchar* str, const OPchar* cmp) {
	ui32 lenA = (ui32)strlen(str);
	ui32 lenB = (ui32)strlen(cmp);
	if (lenA != lenB) return 0;
	return OPmemcmp(str, cmp, lenA) == 0;
}

OPint OPstringStartsWith(OPchar* str, const OPchar* cmp) {
	ui32 lenA = (ui32)strlen(str);
	ui32 lenB = (ui32)strlen(cmp);
	if (lenA < lenB) return 0;
	return OPmemcmp(str, cmp, lenB) == 0;
}

OPint OPstringEndsWith(OPchar* str, const OPchar* cmp) {
	ui32 lenA = (ui32)strlen(str);
	ui32 lenB = (ui32)strlen(cmp);
	if (lenA < lenB) return 0;
	return OPmemcmp(&str[lenA-lenB], cmp, lenB) == 0;
}

void OPstringRemoveFromStart(OPchar* str, i32 size) {
	ui32 i;
	ui32 len = (ui32)strlen(str);
	for (i = 0; i < len - size; i++) {
		str[i] = str[i + size];
	}
	str[len - size] = '\0';
}

OPchar* OPstringGetNonConstant(const OPchar* str) {
	ui32 len, size;
	OPchar* result = NULL;

	len = (ui32)strlen(str);
	size = (len + 1) * sizeof(OPchar);

	result = (OPchar*)OPalloc(size);
	#ifdef OPIFEX_WINDOWS
	strcpy_s(result, size, str);
	#else
	result = strcpy(result, str);
	#endif

	return result;
}

OPchar* OPstringCopy(const OPchar* str) {
	ui32 len, size;
	OPchar* result = NULL;

	//OPlogInfo("copy string");
	//OPlogInfo("copying %s", str);

	len = (ui32)strlen(str);
	size = (len + 1) * sizeof(OPchar);

	result = (OPchar*)OPallocZero(size);
	#ifdef OPIFEX_WINDOWS
	strcpy_s(result, size, str);
	#else
	result = strcpy(result, str);
	#endif
	result[len] = '\0';

	return result;
}

OPchar* OPstringSub(const OPchar* str, ui32 start, ui32 end) {
	ui32 len, subLen;
	OPchar* result = NULL;

	len = (ui32)strlen(str);
	subLen = end - start;

	if(len < subLen) {
		return NULL;
	}


	result = (OPchar*)OPalloc(sizeof(OPchar) * (subLen + 1));
	OPmemcpy(result, &str[start], sizeof(OPchar) * subLen);
	result[subLen] = '\0';

	return result;
}

OPchar* OPstringCreateMerged(const OPchar* str, const OPchar* add) {
	ui32 lenA = (ui32)strlen(str);
	ui32 lenB = (ui32)strlen(add);
	OPchar* result = (OPchar*)OPalloc(lenA + lenB + 1);
	#ifdef OPIFEX_WINDOWS
	strcpy_s(result, lenA + 1, str);
	#else
	result = strcpy(result, str);
	#endif

	#ifdef OPIFEX_WINDOWS
	strcat_s(result, lenA + lenB + 1, add);
    #else
	strcat(result, add);
    #endif
	return result;
}

OPint OPstringContains(const OPchar* str, const OPchar* cmp) {
	ui32 i, strLen, cmpLen;

	strLen = (ui32)strlen(str);
	cmpLen = (ui32)strlen(cmp);
	if(cmpLen < strLen) {
		for(i = 0;i < strLen; i++) {
			if(OPmemcmp(&str[i], cmp, cmpLen) == 0) {
				return i;
			}
		}
	}

	return -1;
}

void OPstringToLower(OPchar* str) {
	OPchar* p = str;
	for ( ; *p; ++p) *p = tolower(*p);
}

void OPstringScan(OPchar* destination, const OPchar* format, ...) {

}

i32 OPstringCount(OPchar* str, OPchar lookFor) {
	ui32 total = 0;
	for (ui32 i = 0; i < strlen(str); i++) {
		if (str[i] == lookFor) {
			total++;
		}
	}
	return total;
}

i32 OPstringFirst(OPchar* str, OPchar lookFor) {
	for (ui32 i = 0; i < strlen(str); i++) {
		if (str[i] == lookFor) {
			return i;
		}
	}
	return -1;
}


i32 OPstringSplit(const OPchar* str, OPchar split, OPchar*** output) {
	ui32 counts = 1;

	// Find all split characters
	for (ui32 i = 0; i < strlen(str); i++) {
		if (str[i] == split) {
			counts++;
		}
	}

	(*output) = OPALLOC(OPchar*, counts);
	OPchar* data = OPstringCopy(str);

	// If there weren't any, return  just the string
	if (counts == 1) {
		(*output)[0] = data;
		return 1;
	}
	

	ui32 ind = 0;
	ui32 strPos = 0;
	for (ui32 i = 0; i < strlen(str); i++) {
		if (str[i] == split) {
			data[i] = NULL; // Replace the split char with NULL
			(*output)[ind++] = &data[strPos];
			strPos = i + 1;
		}
	}
	(*output)[ind++] = &data[strPos];

	return counts;
}
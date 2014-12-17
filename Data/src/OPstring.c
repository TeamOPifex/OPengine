#include "./Data/include/OPstring.h"
#include "./Core/include/OPmemory.h"

OPint OPstringEquals(const OPchar* str, const OPchar* cmp) {
	ui32 lenA = strlen(str);
	ui32 lenB = strlen(cmp);
	if (lenA != lenB) return 0;
	return OPmemcmp(str, cmp, lenA) == 0;
}

OPint OPstringStartsWith(OPchar* str, const OPchar* cmp, i32 size) {
	ui32 lenA = strlen(str);
	ui32 lenB = strlen(cmp);
	if (lenA < size || lenB < size) return 0;
	return OPmemcmp(str, cmp, size) == 0;
}

void OPstringRemoveFromStart(OPchar* str, i32 size) {
	i32 i;
	i32 len = strlen(str);
	for (i = 0; i < len - size; i++) {
		str[i] = str[i + size];
	}
	str[len - size] = '\0';
}

OPchar* OPstringGetNonConstant(const OPchar* str) {
	i32 len;
	OPchar* result = NULL;

	len = strlen(str);

	result = (OPchar*)OPalloc(sizeof(OPchar) * (len + 1));
	strcpy(result, str);
	
	return result;
}

OPchar* OPstringCreateCopy(const OPchar* str) {
	i32 len;
	OPchar* result = NULL;

	len = strlen(str);

	result = (OPchar*)OPalloc(sizeof(OPchar) * (len + 1));
	strcpy(result, str);
	result[len] = '\0';
	
	return result;
}

OPchar* OPstringCreateMerged(const OPchar* str, const OPchar* add) {
	i32 lenA = strlen(str);
	i32 lenB = strlen(add);
	OPchar* result = (OPchar*)OPalloc(lenA + lenB + 1);
	strcpy(result, str);
	strcat(result, add);
	return result;
}




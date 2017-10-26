#include "./Data/include/OPstring.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPlog.h"
#include <ctype.h>

OPchar* _strCopy(const OPchar* str) {
	if (str == NULL) {
		return NULL;
	}

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

	result[len] = '\0';

	return result;
}

OPstring::OPstring(const OPchar* data) {
	Init(_strCopy(data));
}


void OPstring::Init(OPchar* data) {
	_data = _strCopy(data);
	_len = _data == NULL ? 0 : strlen(_data);
}

void OPstring::Init(const OPchar* data) {
	_data = _strCopy(data);
	_len = _data == NULL ? 0 : strlen(_data);
}

void OPstring::Clear() {
	if (_data != NULL) {
		OPfree(_data);
	}
	_data = NULL;
	_len = 0;
}

OPstring::~OPstring() {
	if (_data != NULL) {
		OPfree(_data);
	}
	_data = NULL;
}

bool OPstring::Equals(OPstring* str) {
	if (_len != str->_len) return 0;
	return OPmemcmp(str->_data, _data, _len) == 0;
}

bool OPstring::Equals(const OPchar* str) {
	ui32 lenA = (ui32)strlen(str);
	if (lenA != _len) return 0;
	return OPmemcmp(str, _data, _len) == 0;
}

bool OPstring::StartsWith(OPstring* str) {
	if (_len < str->_len) return 0;
	return OPmemcmp(str->_data, _data, str->_len) == 0;
}

bool OPstring::StartsWith(const OPchar* str) {
	ui32 lenA = (ui32)strlen(str);
	if (_len < lenA) return 0;
	return OPmemcmp(str, _data, lenA) == 0;
}

bool OPstring::EndsWith(OPstring* str) {
	if (_len < str->_len) return 0;
	return OPmemcmp(&_data[_len - str->_len], str->_data, str->_len) == 0;
}

bool OPstring::EndsWith(const OPchar* str) {
	ui32 lenA = (ui32)strlen(str);
	if (_len < lenA) return 0;
	return OPmemcmp(&_data[_len - lenA], str, lenA) == 0;
}


OPstring* OPstring::Create(const OPchar* data) {
	OPstring* result = OPNEW(OPstring);
	result->Init(data);
	return result;
}

OPstring* OPstring::Copy() {
	return OPstring::Create(_data);
}

void OPstring::Add(OPstring* add) {

	OPchar* result = (OPchar*)OPalloc(_len + add->_len + 1);

#ifdef OPIFEX_WINDOWS
	strcpy_s(result, _len + 1, _data);
#else
	result = strcpy(result, _data);
#endif

#ifdef OPIFEX_WINDOWS
	strcat_s(result, _len + add->_len + 1, add->_data);
#else
	strcat(result, add->_data);
#endif

	if (_data != NULL) {
		OPfree(_data);
	}

	Init(result);
}

void OPstring::Add(const OPchar* add) {
	ui32 lenB = (ui32)strlen(add);
	OPchar* result = (OPchar*)OPalloc(_len + lenB + 1);

#ifdef OPIFEX_WINDOWS
	strcpy_s(result, _len + 1, _data);
#else
	result = strcpy(result, _data);
#endif

#ifdef OPIFEX_WINDOWS
	strcat_s(result, _len + lenB + 1, add);
#else
	strcat(result, add);
#endif

	if (_data != NULL) {
		OPfree(_data);
	}

	Init(result);
}


void OPstring::Resize(OPuint size, bool realloc) {
	_data[size] = NULL;
	_len = size;
	if (realloc) {
		void* prev = _data;
		Init(_strCopy(_data));
		OPfree(prev);
	}
}

OPstring* OPstring::Substr(OPuint start, OPuint end) {
	OPuint subLen;

	subLen = end - start;

	if (_len < subLen) {
		return NULL;
	}

	OPchar* result = (OPchar*)OPalloc(sizeof(OPchar) * (subLen + 1));
	OPmemcpy(result, &_data[start], sizeof(OPchar) * subLen);
	result[subLen] = '\0';

	return OPstring::Create(result);
}

OPint OPstring::Contains(OPstring* str) {
	OPuint i;

	if (str->_len < _len) {
		for (i = 0; i < str->_len; i++) {
			if (OPmemcmp(&_data[i], str->_data, str->_len) == 0) {
				return i;
			}
		}
	}

	return -1;
}

OPint OPstring::Contains(const OPchar* cmp) {
	OPuint i, cmpLen;
	cmpLen = strlen(cmp);
	if (cmpLen < _len) {
		for (i = 0; i < _len; i++) {
			if (OPmemcmp(&_data[i], cmp, cmpLen) == 0) {
				return i;
			}
		}
	}

	return -1;
}

void OPstring::ToLower() {
	OPchar* p = _data;
	for (; *p; ++p) *p = tolower(*p);
}

OPint OPstring::IndexOf(OPchar c) {
	for (ui32 i = 0; i < _len; i++) {
		if (_data[i] == c) {
			return i;
		}
	}
	return -1;
}

OPint OPstring::IndexOfLast(OPchar c) {
	OPint result = -1;
	for (OPuint i = 0; i < _len; i++) {
		if (_data[i] == c) {
			result = (OPint)i;
		}
	}
	return result;
}

OPuint OPstring::Count(OPchar c) {
	ui32 total = 0;
	for (ui32 i = 0; i < _len; i++) {
		if (_data[i] == c) {
			total++;
		}
	}
	return total;
}

OPuint OPstring::Split(OPchar split, OPchar*** output) {
	OPuint counts = 1 + Count(split);

	(*output) = OPALLOC(OPchar*, counts);
	OPchar* data = _strCopy(_data);

	// If there weren't any, return  just the string
	if (counts == 1) {
		(*output)[0] = data;
		return 1;
	}

	ui32 ind = 0;
	ui32 strPos = 0;
	for (ui32 i = 0; i < _len; i++) {
		if (_data[i] == split) {
			data[i] = NULL; // Replace the split char with NULL
			(*output)[ind++] = &data[strPos];
			strPos = i + 1;
		}
	}
	(*output)[ind++] = &data[strPos];

	return counts;
}


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
	if (str == NULL) {
		return NULL;
	}

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

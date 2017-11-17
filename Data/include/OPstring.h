#pragma once

#include "./Core/include/OPtypes.h"

OPint OPstringEquals(const OPchar* str, const OPchar* cmp);
OPint OPstringStartsWith(OPchar* str, const OPchar* cmp);
OPint OPstringEndsWith(OPchar* str, const OPchar* cmp);
OPchar* OPstringCopy(const OPchar* str);
OPchar* OPstringCreateMerged(const OPchar* str, const OPchar* add);
void OPstringToLower(OPchar* str);
i32 OPstringCount(OPchar* str, OPchar lookFor);
i32 OPstringFirst(OPchar* str, OPchar lookFor);
i32 OPstringSplit(const OPchar* str, OPchar split, OPchar*** output);
void OPstringScan(OPchar* destination, const OPchar* format, ...);
OPchar* OPstringGetNonConstant(const OPchar* str);
OPchar* OPstringSub(const OPchar* str, ui32 start, ui32 end);
OPint OPstringContains(const OPchar* str, const OPchar* cmp);

void OPstringRemoveFromStart(OPchar* str, i32 size);


struct OPstring {
	OPchar* _data = NULL;
	OPuint _len = 0;

	OPstring() { _data = NULL; _len = 0; }
	OPstring(const OPchar* data);
	~OPstring();

	inline OPuint Length() { return _len; }
	inline OPchar* C_Str() { return _data; }
	
	void Clear();
	bool Equals(OPstring* str);
	bool Equals(const OPchar* str);
	bool StartsWith(OPstring* str);
	bool StartsWith(const OPchar* str);
	bool EndsWith(OPstring* str);
	bool EndsWith(const OPchar* str);
	OPstring* Copy();
	void Add(OPstring* str);
	void Add(const OPchar* str);
	OPstring* Substr(OPuint start, OPuint end);
	OPint Contains(OPstring* str);
	OPint Contains(const OPchar* str);
	void ToLower();
	OPint IndexOf(OPchar c);
	OPint IndexOfLast(OPchar c);
	OPuint Count(OPchar c);
	OPuint Split(OPchar split, OPchar*** output);
	void Resize(OPuint size, bool realloc);
	inline void Resize(OPuint size) { Resize(size, false); }

	void Init(OPchar* data);
	void Init(const OPchar* data);

	static OPstring* Create(const OPchar* data);
};
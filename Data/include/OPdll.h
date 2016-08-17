#pragma once

struct OPdll;
typedef struct OPdll OPdll;

#include "./Core/include/OPtypes.h"
#include "./Core/include/OPmemory.h"

#define OPDLL_LIB lib
#define OPDLL_DYLIB .dylib

#define STRINGIFY_NX(A) #A
#define STRINGIFY(A) STRINGIFY_NX(A)
#define STRING_MERGE_3_NX(A,B,C) A ## B ## C
#define STRING_MERGE_3(A,B,C) STRING_MERGE_NX(A,B,C)
#define OPDLL_PATH(A) "lib" #A ".dylib"

struct OPdll {
#ifdef OPIFEX_WINDOWS
	HMODULE library;
#else
	void* library;
#endif
	const OPchar* path;
#ifdef _DEBUG
	OPuint lastModified;
#endif

	OPdll() { }
	OPdll(const OPchar* path) { Init(path); }

	void Init(const OPchar* path);
	OPint Update();
	void* Symbol(const OPchar* symbol);
	void Destroy();

	inline static OPdll* Create(const OPchar* path) { return OPNEW(OPdll(path)); }

};
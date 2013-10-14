// TODO: Have option to use a FILE* to read from disk rather than memory

#pragma once
#include "./../../Core/include/Types.h"
#include "./../../Core/include/DynamicMemory.h"

#ifndef OPEngine_Data_list
#define OPEngine_Data_list

#if defined(OPIFEX_ANDROID) && defined(__cplusplus)

#else
//#include "./../../Core/include/Core.h"
#endif

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif
	typedef struct{
		ui8* _indices;
		OPint _capacity;
		OPint _size;
		OPint _elementSize;
	} OPlist;

	OPlist* OPlistCreate(OPint capacity, OPint elementSize);
	OPint OPlistDestroy(OPlist* list);
	OPint OPlistPush(OPlist* list, ui8* value);
	ui8* OPlistPop(OPlist* list);
	OPint OPlistInsert(OPlist* list, ui8* value, OPuint index);
	OPint OPlistRemoveAt(OPlist* list, OPuint index);

	ui8* OPlistGet(OPlist* list, OPuint index);
	ui8* OPlistSet(OPlist* list, OPuint index, ui8* value);

	OPint OPlistSize(OPlist* list);

	OPint _oplNextExceedsCap(OPlist* list);
#ifdef __cplusplus
};
//-----------------------------------------------------------------------------
//- C++ Definitions -----------------------------------------------------------
namespace OPEngine{
namespace Data{

}
}
#endif
#endif

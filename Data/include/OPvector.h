// TODO: Have option to use a FILE* to read from disk rather than memory

#pragma once
#include "./../../Core/include/Types.h"
#include "./../../Core/include/DynamicMemory.h"

#ifndef OPEngine_Data_vector
#define OPEngine_Data_vector

#if defined(OPIFEX_ANDROID) && defined(__cplusplus)

#else
//#include "./../../Core/include/Core.h"
#endif

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif
	typedef struct{
		void* items;
		OPint _capacity;
		OPint _size;
		OPint _elementSize;
	} OPvector;

	OPvector* OPvectorCreate(OPint elementSize);
	OPint OPvectorDestroy(OPvector* vector);

	ui8* OPvectorGet(OPvector* vector, OPuint index);

	ui8* OPvectorFront(OPvector* vector);
	ui8* OPvectorBack(OPvector* vector);
	
	OPint OPvectorContains(OPvector* vector, ui8* item, OPint(*cmp)(ui8 *, ui8 *));
	OPint OPvectorIsEmpty(OPvector* vector);
	OPint OPvectorSize(OPvector* vector);

	OPint OPvectorCapacity(OPvector* vector);

	void OPvectorClear(OPvector* vector);
	void OPvectorSet(OPvector* vector, OPint index, ui8* item);
	void OPvectorErase(OPvector* vector, OPint index);
	void OPvectorEraseRange(OPvector* vector, OPint indexFirst, OPint indexLast);

	void OPvectorPush(OPvector* vector, ui8* item);
	void OPvectorPop(OPvector* vector, ui8* item);
	void OPvectorInsert(OPvector* vector, OPint index, ui8* item);

	void OPvectorReserve(OPvector* vector, OPint size);
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

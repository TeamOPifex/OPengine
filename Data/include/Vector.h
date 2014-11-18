// TODO: Have option to use a FILE* to read from disk rather than memory

#pragma once
#include "./Core/include/Types.h"
#include "./Core/include/DynamicMemory.h"

#ifndef OPEngine_Data_vector
#define OPEngine_Data_vector

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif

	//This will be replaced by OPlist 
	typedef struct{
		void* items;
		OPuint _capacity;
		OPuint _size;
		OPuint _elementSize;
	} OPvector;

	OPvector* OPvectorCreate(OPint elementSize);
	void OPvectorDestroy(OPvector* vector);

	ui8* OPvectorGet(OPvector* vector, OPuint index);

	ui8* OPvectorFront(OPvector* vector);
	ui8* OPvectorBack(OPvector* vector);
	
	OPint OPvectorContains(OPvector* vector, ui8* item, OPint(*cmp)(ui8 *, ui8 *));
	OPint OPvectorIsEmpty(OPvector* vector);
	OPint OPvectorSize(OPvector* vector);

	OPint OPvectorCapacity(OPvector* vector);

	void OPvectorClear(OPvector* vector);
	void OPvectorSet(OPvector* vector, OPuint index, ui8* item);
	void OPvectorErase(OPvector* vector, OPuint index);
	void OPvectorEraseRange(OPvector* vector, OPuint indexFirst, OPuint indexLast);

	void OPvectorPush(OPvector* vector, ui8* item);
	void OPvectorPop(OPvector* vector, ui8* item);
	void OPvectorInsert(OPvector* vector, OPuint index, ui8* item);

	void OPvectorReserve(OPvector* vector, OPuint size);
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

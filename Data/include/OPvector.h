#ifndef OPEngine_Data_vector
#define OPEngine_Data_vector

#include "./Core/include/OPtypes.h"
#include "./Core/include/OPmemory.h"

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

	EXPORT_METHOD OPvector* OPvectorCreate(OPint elementSize);
	EXPORT_METHOD void OPvectorDestroy(OPvector* vector);

	EXPORT_METHOD ui8* OPvectorGet(OPvector* vector, OPuint index);

	EXPORT_METHOD ui8* OPvectorFront(OPvector* vector);
	EXPORT_METHOD ui8* OPvectorBack(OPvector* vector);
	
	EXPORT_METHOD OPint OPvectorContains(OPvector* vector, ui8* item, OPint(*cmp)(ui8 *, ui8 *));
	EXPORT_METHOD OPint OPvectorIsEmpty(OPvector* vector);
	EXPORT_METHOD OPint OPvectorSize(OPvector* vector);

	EXPORT_METHOD OPint OPvectorCapacity(OPvector* vector);

	EXPORT_METHOD void OPvectorClear(OPvector* vector);
	EXPORT_METHOD void OPvectorSet(OPvector* vector, OPuint index, ui8* item);
	EXPORT_METHOD void OPvectorErase(OPvector* vector, OPuint index);
	EXPORT_METHOD void OPvectorEraseRange(OPvector* vector, OPuint indexFirst, OPuint indexLast);

	EXPORT_METHOD void OPvectorPush(OPvector* vector, ui8* item);
	EXPORT_METHOD void OPvectorPop(OPvector* vector, ui8* item);
	EXPORT_METHOD void OPvectorInsert(OPvector* vector, OPuint index, ui8* item);

	EXPORT_METHOD void OPvectorReserve(OPvector* vector, OPuint size);

#ifdef __cplusplus
};
#endif

#endif

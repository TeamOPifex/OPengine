#ifndef OPEngine_Data_list
#define OPEngine_Data_list

#include "./Core/include/OPtypes.h"
#include "./Core/include/OPmemory.h"

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif

	/*
	 * An OPlist
	*/
	struct OPlist {
		ui8* _indices;
		OPuint _capacity;
		OPuint _size;
		OPuint _elementSize;
	};
	typedef struct OPlist OPlist;

	/* Creates a new OPlist
	 * @param Starting count to allocate for the list
	 * @param Size of each element that will be contained in the OPlist
	 * @return A new OPlist
	*/
	OPlist* OPlistCreate(OPint capacity, OPint elementSize);

	/* Destroys an OPlist
	 * @param The OPlist to destroy
	 * @return Success Result
	*/
	OPint OPlistDestroy(OPlist* list);

	/* Copies an element into the OPlist
	 * The value is mem copied into the list so the value passed can be safely freed
	 * @param list The OPlist to push the value into
	 * @param value A pointer to the value (of elementSize which was specified on creation of the OPlist)
	 * @return The index in the OPlist of the inserted element
	*/
	OPint OPlistPush(OPlist* list, ui8* value);

	/* Pops the last element of the OPlist
	 * @param list The OPlist to pop
	 * @return Value of the last element (This will be overwritten on the next OPlistPush)
	*/
	ui8* OPlistPop(OPlist* list);

	/* Peeks at the last element of the OPlist
	 * @param list The OPlist to peek
	 * @return Value of the last element (This will be different on the next OPlistPush)
	*/
	ui8* OPlistPeek(OPlist* list);

	/* Inserts an element at an index to an OPlist
	 * @param list The OPlist to insert into
	 * @param value The value to insert
	 * @param index The index into the OPlist to insert the value
	 * @return Success Result
	*/
	OPint OPlistInsert(OPlist* list, ui8* value, OPuint index);

	/* Removes an element at an index of an OPlist
	 * @param list The OPlist to remove from
	 * @param index The index into the OPlist to remove
	 * @return Success Result
	*/
	OPint OPlistRemoveAt(OPlist* list, OPuint index);


	/* Gets an element at an index of an OPlist
	 * @param list The OPlist to get an element from
	 * @param index The index into the OPlist to get a value
	 * @return Value of the element
	*/
	ui8* OPlistGet(OPlist* list, OPuint index);

	/* Sets an element at an index of an OPlist
	 * This will overwrite anything at the specified index
	 * @param list The OPlist to set the value into
	 * @param index The index into the OPlist to set
	 * @param value The value to set the index to
	 * @return Value of the indexed element
	*/
	ui8* OPlistSet(OPlist* list, OPuint index, ui8* value);

	/* Current size of an OPlist
	 * @param list The OPlist to find the size of
	 * @param Total count of elements
	*/
	OPint OPlistSize(OPlist* list);

#ifdef __cplusplus
};
#endif

#endif

#pragma once

struct OPlist;
typedef struct OPlist OPlist;

#include "./Core/include/OPmemory.h"

/*
	* An OPlist
*/
struct OPlist {
	ui8* _indices;
	OPuint _capacity;
	OPuint _size;
	OPuint _elementSize;

	OPlist() {}
	OPlist(OPuint capacity, OPuint elementSize) { Init(capacity, elementSize); }

	void Init(OPuint capacity, OPuint elementSize);

	/* Copies an element into the OPlist
	* The value is mem copied into the list so the value passed can be safely freed
	* @param list The OPlist to push the value into
	* @param value A pointer to the value (of elementSize which was specified on creation of the OPlist)
	* @return The index in the OPlist of the inserted element
	*/
	OPuint Push(ui8* value);

	/* Pops the last element of the OPlist
	* @param list The OPlist to pop
	* @return Value of the last element (This will be overwritten on the next OPlistPush)
	*/
	ui8* Pop();

	/* Peeks at the last element of the OPlist
	* @param list The OPlist to peek
	* @return Value of the last element (This will be different on the next OPlistPush)
	*/
	inline ui8* Peek() {
		return _indices + (_size - 1) * _elementSize;
	}

	/* Inserts an element at an index to an OPlist
	* @param list The OPlist to insert into
	* @param value The value to insert
	* @param index The index into the OPlist to insert the value
	* @return Success Result
	*/
	bool Insert(ui8* value, OPuint index);

	/* Removes an element at an index of an OPlist
	* @param list The OPlist to remove from
	* @param index The index into the OPlist to remove
	* @return Success Result
	*/
	bool Remove(OPuint index);


	/* Gets an element at an index of an OPlist
	* @param list The OPlist to get an element from
	* @param index The index into the OPlist to get a value
	* @return Value of the element
	*/
	inline ui8* Get(OPuint index) { 
		return _indices + (index * _elementSize); 
	}

	/* Sets an element at an index of an OPlist
	* This will overwrite anything at the specified index
	* @param list The OPlist to set the value into
	* @param index The index into the OPlist to set
	* @param value The value to set the index to
	* @return Value of the indexed element
	*/
	ui8* Set(OPuint index, ui8* value);

	/* Current size of an OPlist
	* @param list The OPlist to find the size of
	* @param Total count of elements
	*/
	OPuint Size() { return _size; }

	/* Destroys an OPlist
	* @param The OPlist to destroy
	* @return Success Result
	*/
	void Destroy();

	// Destroy and then free this linked list
	inline void Free() { 
		Destroy(); 
		OPfree(this); 
	}

	/* Creates a new OPlist
	* @param Starting count to allocate for the list
	* @param Size of each element that will be contained in the OPlist
	* @return A new OPlist
	*/
	inline static OPlist* Create(OPuint capacity, OPuint elementSize) { return OPNEW(OPlist(capacity, elementSize)); }
};

#pragma once

struct OPminHeap;
typedef struct OPminHeap OPminHeap;

#include "./Core/include/OPmemory.h"

struct OPminHeap {
	OPint* _indices;
	OPuint _capacity;
	OPuint _size;

	OPminHeap() { }
	OPminHeap(OPuint capacity) { Init(capacity); }

	void Init(OPuint capacity);

	/* Puts an element into an OPminHeap
	* @return Success Result
	*/
	bool Push(OPint value);

	/* Pops an element off the top of an OPminHeap
	* @return The Index of the element popped off
	*/
	OPint Pop();

	/* Looks at the top element of an OPminHeap without popping it off
	* @param heap The OPminHeap to look at
	* @return Index of the element
	*/
	OPint Peek();

	void Destroy();

	/* Size of the OPminHeap
	* @return Total size of the OPminHeap
	*/
	inline OPuint Size() { return _size; }

	inline void Free() {
		Destroy();
		OPfree(this);
	}

	/* Creates an OPminHeap
	* @param capacity Number of elements to store
	* @return return The Created OPminHeap
	*/
	inline static OPminHeap* Create(OPuint capacity) { return OPNEW(OPminHeap(capacity)); }
};



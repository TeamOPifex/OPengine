#pragma once

struct OPlinkedList;
typedef struct OPlinkedList OPlinkedList;

#include "./Data/include/OPlinkedListNode.h"
#include "./Core/include/OPtypes.h"
#include "./Core/include/OPmemory.h"

struct OPlinkedList {
	OPuint _size;
	OPlinkedListNode* First;
	OPlinkedListNode* Last;

	OPlinkedList() { Init(); }

	void Init();

	// Inserts a node into the first position of an OPlinkedList
	OPlinkedListNode* InsertFirst(void* data);
	// Inserts a node into the last position of an OPlinkedList
	OPlinkedListNode* InsertLast(void* data);
	// Removes an OPllNode from an OPlinkedList
	void* Remove(OPlinkedListNode* node);
	// Gets the number of elements stored in the OPlinkedList
	OPuint Size() { return _size; }
	void Destroy();

	static inline OPlinkedList* Create() { 
		return OPNEW(OPlinkedList()); 
	}
};
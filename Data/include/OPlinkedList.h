#pragma once

struct OPlinkedList;
struct OPlinkedListNode;

typedef struct OPlinkedList OPlinkedList;
typedef struct OPlinkedListNode OPlinkedListNode;

#include "./Core/include/OPtypes.h"
#include "./Core/include/OPmemory.h"

struct OPlinkedListNode {
	OPlinkedListNode* Prev;
	OPlinkedListNode* Next;
	void* Data;
};

struct OPlinkedList {
	OPuint _size;
	OPlinkedListNode* First;
	OPlinkedListNode* Last;

	OPlinkedList() { _size = 0; First = NULL; Last = NULL; }

	// Inserts a node into the first position of an OPlinkedList
	OPlinkedListNode* InsertFirst(void* data);
	// Inserts a node into the last position of an OPlinkedList
	OPlinkedListNode* InsertLast(void* data);
	// Removes an OPllNode from an OPlinkedList
	void* Remove(OPlinkedListNode* node);
	// Gets the number of elements stored in the OPlinkedList
	OPuint Size() { return _size; }
	void Destroy();

	static inline OPlinkedList* Create() { return OPNEW(OPlinkedList()); }
};
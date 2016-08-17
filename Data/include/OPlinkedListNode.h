#pragma once

struct OPlinkedListNode;
typedef struct OPlinkedListNode OPlinkedListNode;

struct OPlinkedListNode {
	OPlinkedListNode* Prev;
	OPlinkedListNode* Next;
	void* Data;
};
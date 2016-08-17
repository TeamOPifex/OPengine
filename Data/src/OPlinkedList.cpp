#include "./Data/include/OPlinkedList.h"

void OPlinkedList::Init() {
	_size = 0; 
	First = NULL; 
	Last = NULL;
}

void OPlinkedList::Destroy(){
	OPlinkedListNode* temp;
	OPlinkedListNode* node = First;
	while (node) {
		temp = node;
		node = node->Next;
		Remove(temp);
	}
}

OPlinkedListNode* OPlinkedList::InsertFirst(void* data){
	OPlinkedListNode* node = OPNEW(OPlinkedListNode);
	node->Data = data;
	node->Next = node->Prev = NULL;
	++_size;

	if(First == NULL){
		Last = node;
	}
	else{
		node->Next = First;
		First->Prev = node;
	}

	First = node;

	return node;
}

OPlinkedListNode* OPlinkedList::InsertLast(void* data){
	OPlinkedListNode* node = OPNEW(OPlinkedListNode);
	node->Data = data;
	node->Prev = node->Next = NULL;
	_size++;

	if(Last == NULL){
		First = node;
	}
	else{
		node->Prev = Last;
		Last->Next = node;
	}

	Last = node;

	return node;
}

void* OPlinkedList::Remove(OPlinkedListNode* node){

	OPlinkedListNode *next = node->Next, *prev = node->Prev;
	void* data = node->Data;

	if(prev) prev->Next = next;
	else First = next;

	if(next) next->Prev = prev;
	else Last = prev;

	OPfree(node);

	_size--;

	return data;
}
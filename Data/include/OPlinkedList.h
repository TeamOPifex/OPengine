#ifndef OPEngine_Data_LinkedList
#define OPEngine_Data_LinkedList

#include "./Core/include/OPtypes.h"
#include "./Core/include/OPmemory.h"

#if defined(OPIFEX_ANDROID) && defined(__cplusplus)
	#include "./Core/include/OPmemory.h"
#endif

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif

struct OPllNode_def;
typedef struct OPllNode_def OPllNode;

struct OPllNode_def {	
	OPllNode *Prev;
	OPllNode *Next;
	void* Data;
};

typedef struct{
	OPint _size;
	OPllNode* First;
	OPllNode* Last;
} OPlinkedList;

/* Creates an OPlinkedList
 * @return Newly allocated OPlinkedList
*/
EXPORT_METHOD OPlinkedList* OPllCreate();

/* Destroys an OPlinkedList
 * @param list The OPlinkedList to destroy
 * @return Success Result
*/
EXPORT_METHOD OPint OPllDestroy(OPlinkedList* list);

/* Inserts a node into the first position of an OPlinkedList
 * @param list The OPlinkedList to insert into
 * @param data The pointer to put into the linked list
 * @return The OPllNode that was inserted
*/
EXPORT_METHOD OPllNode* OPllInsertFirst(OPlinkedList* list, void* data);

/* Inserts a node into the last position of an OPlinkedList
 * @param list The OPlinkedList to insert into
 * @param data The pointer to put into the linked list
 * @return The OPllNode that was inserted
*/
EXPORT_METHOD OPllNode* OPllInsertLast(OPlinkedList* list, void* data);

/* Removes an OPllNode from an OPlinkedList
 * @param list The OPlinkedList to remove from
 * @param toRemove The OPllNode to remove from the OPlinkedList
 * @return The pointer to the data of the OPllNode removed
*/
EXPORT_METHOD void* OPllRemove(OPlinkedList* list, OPllNode* toRemove);

/* Gets the number of elements stored in the OPlinkedList
 * @param list The OPlinkedList to get the count from
 * @return The number of elements in the OPlinkedList
*/
EXPORT_METHOD OPint OPllGetSize(OPlinkedList* list);

#ifdef __cplusplus
};
#endif

#endif

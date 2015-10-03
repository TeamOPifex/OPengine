#ifndef OPEngine_Data_MinHeap
#define OPEngine_Data_MinHeap

#include "./Core/include/OPtypes.h"
#include "./Core/include/OPmemory.h"

#if defined(OPIFEX_ANDROID) && defined(__cplusplus)

#else
//#include "./../../Core/include/Core.h"
#endif

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif
	struct OPminHeap{
		OPint* _indices;
		OPuint _capacity;
		OPuint _size;
	};
	typedef struct OPminHeap OPminHeap;

	/* Creates an OPminHeap
	 * @param capacity Number of elements to store
	 * @return return The Created OPminHeap
	*/
	OPminHeap* OPminHeapCreate(OPuint capacity);

	/* Destroys an OPminHeap
	 * @param heap The OPminHeap to destroy
	 * @return Success Result
	*/
	OPint OPminHeapDestroy(OPminHeap* heap);

	/* Puts an element into an OPminHeap
	 * @param heap The OPminHeap to push an element into
	 * @return Success Result
	*/
	OPint OPminHeapPush(OPminHeap* heap, OPint value);

	/* Pops an element off the top of an OPminHeap
	 * @param heap The OPminHeap to pop an element
	 * @return The Index of the element popped off
	*/
	OPint OPminHeapPop(OPminHeap* heap);

	/* Looks at the top element of an OPminHeap without popping it off
	 * @param heap The OPminHeap to look at
	 * @return Index of the element
	*/
	OPint OPminHeapPeek(OPminHeap* heap);

	/* Size of the OPminHeap
	 * @param heap The OPminHeap to find the size of
	 * @return Total size of the OPminHeap
	*/
	OPint OPminHeapSize(OPminHeap* heap);

#ifdef __cplusplus
};
#endif

#endif

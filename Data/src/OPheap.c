#include "./../include/OPheap.h"

//-----------------------------------------------------------------------------
OPminHeap* OPminHeapCreate(OPint capacity){
	OPminHeap* heap = (OPminHeap*)OPalloc(sizeof(OPminHeap));
	heap->_indices = (OPint*)OPalloc(sizeof(OPint) * capacity);
	heap->_size = 0;
	heap->_capacity = capacity;
}
//-----------------------------------------------------------------------------
OPint OPminHeapDestroy(OPminHeap* heap){
	OPfree(heap->_indices);
	OPfree(heap);
	return 1;
}
//-----------------------------------------------------------------------------
OPint OPminHeapPush(OPminHeap* heap, OPint value);
OPint OPminHeapPop(OPminHeap* heap);
OPint OPminHeapPeek(OPminHeap* heap){
	OPint size = heap->_size;
	if(size == 0) return 0;
	return heap->_indices[size - 1];
}
//-----------------------------------------------------------------------------
void _bubbleDown(OPminHeap* heap, OPint i){
	OPint r = _leftChild(i);
	OPint l = _rightChild(i);
	OPint index = heap->_size;
	OPint* arr = heap->_indices;

	if(l < index && r <index){
		if(arr[r] <= arr[l]){
			if(arr[r] <= arr[i]){
				_swap(&arr[r], &arr[l]);
				_bubbleDown(heap, r);
			}
		}
		else{
			if(arr[l] <= arr[i]){
				_swap(&arr[l], &arr[i]);
				_bubbleDown(heap, l);
			}
		}
	}
}
//-----------------------------------------------------------------------------
void _bubbleUp(OPint* arr, OPint i){
	if(i > 1){
		OPint p = _parent(i);

		if(arr[p] >= arr[i]){
			_swap(&arr[p], &arr[i]);
			_bubbleUp(arr, p);
		}
	}
}
//-----------------------------------------------------------------------------
//- C++ Definitions -----------------------------------------------------------
#ifdef __cplusplus // compile the C++ class
using namespace OPEngine::Data;

#endif

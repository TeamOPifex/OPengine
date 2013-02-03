#include "./../include/OPheap.h"

//-----------------------------------------------------------------------------
OPminHeap* OPminHeapCreate(OPint capacity){
	OPminHeap* heap = (OPminHeap*)OPalloc(sizeof(OPminHeap));
	OPint i = 0;
	heap->_indices = (OPint*)OPalloc(sizeof(OPint) * (capacity + 1));
	
	for(i = 0; i < capacity + 1; i++){
		heap->_indices[i] = 0;
	}

	heap->_size = 1;
	heap->_capacity = capacity + 1;

	return heap;
}
//-----------------------------------------------------------------------------
OPint OPminHeapDestroy(OPminHeap* heap){
	OPfree(heap->_indices);
	OPfree(heap);
	return 1;
}
//-----------------------------------------------------------------------------
OPint OPminHeapPush(OPminHeap* heap, OPint value){
	if(heap->_size < heap->_capacity){
		OPint i = heap->_size;
		heap->_indices[i] = value;
		_bubbleUp(heap->_indices, i);
		heap->_size++;
		return 1;
	}
	else return 0;
}
//-----------------------------------------------------------------------------
OPint OPminHeapPop(OPminHeap* heap){
	OPint* arr = heap->_indices;
	OPint out = arr[1];

	if(heap->_size <= 1) return 0;

	_swap(&arr[1], &arr[heap->_size - 1]);
	heap->_size--;
	_bubbleDown(heap, 1);

	return out;
}
//-----------------------------------------------------------------------------
OPint OPminHeapPeek(OPminHeap* heap){
	if(heap->_size == 1) return 0;
	return heap->_indices[1];
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
				_swap(&arr[r], &arr[i]);
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
OPint OPminHeapSize(OPminHeap* heap){
	return heap->_size - 1;
}
//-----------------------------------------------------------------------------
OPint _parent(OPint i){
    OPint r = i % 2;

    if (r == 0) return i / 2;
    return (i - 1) / 2;
}
//-----------------------------------------------------------------------------
OPint _leftChild(OPint i){
    return i * 2;
}
//-----------------------------------------------------------------------------
OPint _rightChild(OPint i){
    return (i * 2) + 1;
}
//-----------------------------------------------------------------------------
void _swap(OPint* a, OPint* b){
	OPint temp = *a;
	*a = *b;
	*b = temp;
}
//-----------------------------------------------------------------------------
//- C++ Definitions -----------------------------------------------------------
#ifdef __cplusplus // compile the C++ class
using namespace OPEngine::Data;

#endif

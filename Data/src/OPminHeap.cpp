#include "./Data/include/OPminHeap.h"

//-----------------------------------------------------------------------------
OPminHeap* OPminHeapCreate(OPuint capacity){
	OPminHeap* heap = (OPminHeap*)OPalloc(sizeof(OPminHeap));
	OPuint i = 0;
	heap->_indices = (OPint*)OPalloc(sizeof(OPint) * (capacity + 1));
	
	for(i = 1; i < capacity + 1; i++){
		heap->_indices[i] = 0;
	}

	// Make index 0 the most negative integer possible
	for(i = sizeof(OPint); i--;){
		heap->_indices[0] |= 0xFF << (i * 8);
	}

	heap->_size = 0;
	heap->_capacity = capacity;

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
		OPuint now;
		heap->_size++;
		heap->_indices[heap->_size] = value; /*Insert in the last place*/
        	/*Adjust its position*/
        	now = heap->_size;
        	while((heap->_indices[(now >> 1)]) > value){
                	heap->_indices[now] = heap->_indices[(now >> 1)];
                	now >>= 1;
        	}
		heap->_indices[now] = value;
		return 1;
	}
	return 0;
}

//-----------------------------------------------------------------------------
OPint OPminHeapPop(OPminHeap* heap){
	OPint minElement, lastElement, child;
	OPuint now;
	minElement = heap->_indices[1];
	lastElement = heap->_indices[heap->_size--];
    /* now refers to the index at which we are now */
    for(now = 1; (now << 1) <= heap->_size ; now = child){
            /* child is the index of the element which is minimum among both the children */ 
            /* Indexes of children are i*2 and i*2 + 1*/
            child = (now << 1);
            /*child!=heapSize beacuse heap[heapSize+1] does not exist, which means it has only one 
                child */
			if(child != heap->_size && heap->_indices[child + 1] < heap->_indices[child] ){
                    child++;
            }
            /* To check if the last element fits ot not it suffices to check if the last element
                is less than the minimum element among both the children*/
			if(lastElement > heap->_indices[child])
            {
                    heap->_indices[now] = heap->_indices[child];
            }
            else /* It fits there */
            {
                    break;
            }
    }
    heap->_indices[now] = lastElement;
    return minElement;
}

//-----------------------------------------------------------------------------
OPint OPminHeapPeek(OPminHeap* heap){
	if(heap->_size == 1) return -1;
	return heap->_indices[1];
}

//-----------------------------------------------------------------------------
OPint OPminHeapSize(OPminHeap* heap){
	return heap->_size;
}

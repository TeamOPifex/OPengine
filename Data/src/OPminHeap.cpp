#include "./Data/include/OPminHeap.h"

void OPminHeap::Init(OPuint capacity){
	OPuint i = 0;
	
	_indices = (OPint*)OPalloc(sizeof(OPint) * (capacity + 1));
	
	for(i = 1; i < capacity + 1; i++){
		_indices[i] = 0;
	}

	// Make index 0 the most negative integer possible
	for(i = sizeof(OPint); i--;){
		_indices[0] |= 0xFF << (i * 8);
	}

	_size = 0;
	_capacity = capacity;
}

void OPminHeap::Destroy(){
	OPfree(_indices);
}

bool OPminHeap::Push(OPint value){
	if(_size < _capacity){
		OPuint now;
		_size++;
		_indices[_size] = value; /*Insert in the last place*/
        	/*Adjust its position*/
        	now = _size;
        	while((_indices[(now >> 1)]) > value){
                	_indices[now] = _indices[(now >> 1)];
                	now >>= 1;
        	}
		_indices[now] = value;
		return true;
	}
	return false;
}

OPint OPminHeap::Pop(){
	OPint minElement, lastElement, child;
	OPuint now;
	minElement = _indices[1];
	lastElement = _indices[_size--];
    /* now refers to the index at which we are now */
    for(now = 1; (now << 1) <= _size ; now = child){
            /* child is the index of the element which is minimum among both the children */ 
            /* Indexes of children are i*2 and i*2 + 1*/
            child = (now << 1);
            /*child!=heapSize beacuse heap[heapSize+1] does not exist, which means it has only one 
                child */
			if(child != _size && _indices[child + 1] < _indices[child] ){
                    child++;
            }
            /* To check if the last element fits ot not it suffices to check if the last element
                is less than the minimum element among both the children*/
			if(lastElement > _indices[child])
            {
                    _indices[now] = _indices[child];
            }
            else /* It fits there */
            {
                    break;
            }
    }
    _indices[now] = lastElement;
    return minElement;
}

OPint OPminHeap::Peek(){
	if(_size == 1) return -1;
	return _indices[1];
}
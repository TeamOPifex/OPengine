#include "./Data/include/OPlist.h"

OPint _oplNextExceedsCap(OPlist* list){
	OPint eleSize = list->_elementSize;
	if((list->_size + 1) * eleSize >= list->_capacity * eleSize)
		return 1;
	else
		return 0;
}

void OPlist::Init(OPuint capacity, OPuint elementSize) {
	this->_capacity = capacity;
	this->_elementSize = elementSize;
	this->_size = 0;
	this->_indices = (ui8*)OPalloc(sizeof(ui8) * elementSize * capacity);
}

OPuint OPlist::Push(ui8* value){
	OPint eleSize = _elementSize;

	if(_oplNextExceedsCap(this)){
		_capacity *= 2;
		_indices = (ui8*)OPrealloc(_indices, eleSize * _capacity);
	}

	// copy bytes into the array
	OPmemcpy((_indices + (_size * eleSize)), value, eleSize);

	_size++;

	return _size - 1;
}

ui8* OPlist::Pop(){
	_size--;
	return _indices + _size * _elementSize;
}

bool OPlist::Insert(ui8* value, OPuint index){
	OPuint eleSize = _elementSize;
	OPuint i;

	// expand the array if needed
	if(_oplNextExceedsCap(this)){
		_capacity *= 2;
		_indices = (ui8*)OPrealloc(_indices, eleSize * _capacity);
	}
	_size++;

	// shift elements to the right one
	for(i = _size; i > index; i--){
		Set(i, Get(i - 1));
	}

	// set the value
	Set(index, value);

	return true;
}

bool OPlist::Remove(OPuint index){
	OPuint i;
	_size--;

	// shift all the elements to the left copying over the value at index
	for(i = index; i < _size; i++){
		Set(i, Get(i + 1));
	}

	return true;
}

ui8* OPlist::Set(OPuint index, ui8* value){
	OPint eleSize = _elementSize, i = 0;

	// copy bytes into the array
	for(i = eleSize; i--;)
		_indices[index * eleSize + i] = value[i];

	return _indices + (index * _elementSize);
}

void OPlist::Destroy() {
	OPfree(_indices);
}
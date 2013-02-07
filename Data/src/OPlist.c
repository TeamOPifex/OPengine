#include "./../include/OPlist.h"

//-----------------------------------------------------------------------------
OPlist* OPlistCreate(OPint capacity, OPint elementSize){
	OPlist* list = (OPlist*)OPalloc(sizeof(OPlist));

	list->_capacity = capacity;
	list->_elementSize = elementSize;
	list->_indices = (ui8*)OPalloc(sizeof(ui8) * elementSize * capacity);
	list->_size = 0;

	return list;
}
//-----------------------------------------------------------------------------
OPint OPlistDestroy(OPlist* list){
	OPfree(list->_indices);
	OPfree(list);
	return 1;
}
//-----------------------------------------------------------------------------
OPint OPlistPush(OPlist* list, ui8* value){
	OPint eleSize = list->_elementSize;
	OPint i;

	if(_oplNextExceedsCap(list)){
		list->_capacity *= 2;
		list->_indices = (ui8*)OPrealloc(list->_indices, eleSize * list->_capacity);
	}

	// copy bytes into the array
	for(i = eleSize; i--;)
		list->_indices[list->_size * eleSize + i] = value[i];

	list->_size++;

	return list->_size - 1;
}
//-----------------------------------------------------------------------------
ui8* OPlistPop(OPlist* list){
	list->_size--;
	return list->_indices + list->_size * list->_elementSize;
}
//-----------------------------------------------------------------------------
OPint OPlistInsert(OPlist* list, ui8* value, OPuint index){
	OPint eleSize = list->_elementSize;
	OPint i;

	// expand the array if needed
	if(_oplNextExceedsCap(list)){
		list->_capacity *= 2;
		list->_indices = (ui8*)OPrealloc(list->_indices, eleSize * list->_capacity);
	}
	list->_size++;

	// shift elements to the right one
	for(i = list->_size; i > index; i--){
		OPlistSet(list, i, OPlistGet(list, i - 1));
	}

	// set the value
	OPlistSet(list, index, value);

	return 1;
}
//-----------------------------------------------------------------------------
OPint OPlistRemoveAt(OPlist* list, OPuint index){
	OPint i;
	list->_size--;

	// shift all the elements to the left copying over the value at index
	for(i = index; i < list->_size; i++){
		OPlistSet(list, i, OPlistGet(list, i+1));
	}
	return 1;
}
//-----------------------------------------------------------------------------
ui8* OPlistGet(OPlist* list, OPuint index){
	return list->_indices + (index * list->_elementSize);
}
//-----------------------------------------------------------------------------
ui8* OPlistSet(OPlist* list, OPuint index, ui8* value){
	OPint eleSize = list->_elementSize, i = 0;

	// copy bytes into the array
	for(i = eleSize; i--;)
		list->_indices[index * eleSize + i] = value[i];

	return list->_indices + (index * list->_elementSize);
}
//-----------------------------------------------------------------------------
OPint OPlistSize(OPlist* list){
	return list->_size;
}
//-----------------------------------------------------------------------------
OPint _oplNextExceedsCap(OPlist* list){
	OPint eleSize = list->_elementSize;
	if((list->_size + 1) * eleSize >= list->_capacity * eleSize)
		return 1;
	else
		return 0;
}
//-----------------------------------------------------------------------------
//- C++ Definitions -----------------------------------------------------------
#ifdef __cplusplus // compile the C++ class
using namespace OPEngine::Data;

#endif

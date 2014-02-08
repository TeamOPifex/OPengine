#include "./../include/Vector.h"

OPvector* OPvectorCreate(OPint elementSize) {
	OPvector* self = (OPvector*)OPalloc(sizeof(OPvector));
	
	if (!self) {
		// Error
	}

	self->_elementSize = elementSize;
	self->_size = 0;
	self->_capacity = 1;
	self->items = OPalloc(self->_elementSize * self->_capacity);

	return self;
}

OPint OPvectorDestroy(OPvector* vector) {
	OPfree(vector->items);
	OPfree(vector);
}

ui8* OPvectorGet(OPvector* vector, OPuint index) {
	return (ui8*)vector->items + index * vector->_elementSize;
}

ui8* OPvectorFront(OPvector* vector) {
	return OPvectorGet(vector, 0);
}
ui8* OPvectorBack(OPvector* vector) {
	return OPvectorGet(vector, vector->_size - 1);
}

OPint OPvectorContains(OPvector* vector, ui8* item, OPint(*cmp)(ui8 *, ui8 *)) {
	OPint i;
	for (i = 0; i < vector->_size; ++i){
		if ((*cmp)(item, OPvectorGet(vector, i)) == 0) {
			return 1;
		}
	}
	return 0;
}
OPint OPvectorIsEmpty(OPvector* vector) {
	return vector->_size == 0;
}
OPint OPvectorSize(OPvector* vector) {
	return vector->_size;
}

OPint OPvectorCapacity(OPvector* vector) {
	return vector->_capacity;
}

void OPvectorClear(OPvector* vector)  {
	vector->_size = 0;
}
void OPvectorSet(OPvector* vector, OPint index, ui8* item) {
	OPmemcpy((ui8*)(vector->items) + index * vector->_elementSize, item, vector->_elementSize);
}
void OPvectorErase(OPvector* vector, OPint index) {
	OPvectorEraseRange(vector, index, index + 1);
}
void OPvectorEraseRange(OPvector* vector, OPint indexFirst, OPint indexLast) {
	OPmemmove(
		(i8*)(vector->items) + indexFirst * vector->_elementSize,
		(i8*)(vector->items) + indexLast * vector->_elementSize,
		(vector->_size - indexLast) * vector->_elementSize
		);
	vector->_size -= (indexLast - indexFirst);
}

void OPvectorPush(OPvector* vector, ui8* item) {
	OPvectorInsert(vector, vector->_size, item);
}

void OPvectorPop(OPvector* vector, ui8* item) {
	vector->_size--;
}

void OPvectorInsert(OPvector* vector, OPint index, ui8* item) {
	if (vector->_capacity <= vector->_size) {
		OPvectorReserve(vector, 2 * vector->_capacity);
	}
	if (index < vector->_size) {
		OPmemmove(
			(ui8*)(vector->items) + (index + 1) * vector->_elementSize,
			(ui8*)(vector->items) + (index + 0) * vector->_elementSize,
			(vector->_size - index) * vector->_elementSize
			);
	}
	vector->_size++;
	OPvectorSet(vector, index, item);
}
void OPvectorReserve(OPvector* vector, OPint size) {
	if (vector->_capacity < size) {
		vector->items = OPrealloc(vector->items, size * vector->_elementSize);
		vector->_capacity = size;
	}
}
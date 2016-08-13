#include "./Data/include/OPvector.h"

void OPvector::Init(OPuint elementSize) {
	_elementSize = elementSize;
	_size = 0;
	_capacity = 16;
	items = OPalloc(_elementSize * _capacity);
}

void OPvector::Destroy() {
	OPfree(items);
}

OPint OPvector::Contains(ui8* item, OPint(*cmp)(ui8 *, ui8 *)) {
	OPuint i;
	for (i = 0; i < _size; ++i){
		if ((*cmp)(item, Get(i)) == 0) {
			return 1;
		}
	}
	return 0;
}

void OPvector::EraseRange(OPuint indexFirst, OPuint indexLast) {
	OPmemmove(
		(i8*)(items) + indexFirst * _elementSize,
		(i8*)(items) + indexLast * _elementSize,
		(_size - indexLast) * _elementSize
		);
	_size -= (indexLast - indexFirst);
}

void OPvector::Insert(OPuint index, ui8* item) {
	if (_capacity <= _size) {
		Reserve(2 * _capacity);
	}
	if (index < _size) {
		OPmemmove(
			(ui8*)(items) + (index + 1) * _elementSize,
			(ui8*)(items) + (index + 0) * _elementSize,
			(_size - index) * _elementSize
			);
	}
	_size++;
	Set(index, item);
}

void OPvector::Reserve(OPuint size) {
	if (_capacity < size) {
		items = OPrealloc(items, size * _elementSize);
		_capacity = size;
	}
}
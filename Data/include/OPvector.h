#pragma once

struct OPvector;
typedef struct OPvector OPvector;

#include "./Core/include/OPmemory.h"

//This will be replaced by OPlist
struct OPvector {
	void* items;
	OPuint _capacity;
	OPuint _size;
	OPuint _elementSize;

	OPvector() { }
	OPvector(OPuint elementSize) { Init(elementSize); }

	void Init(OPuint elementSize);
	OPint Contains(ui8* item, OPint(*cmp)(ui8 *, ui8 *));
	void EraseRange(OPuint indexFirst, OPuint indexLast);
	void Insert(OPuint index, ui8* item);
	void Reserve(OPuint size);
	void Destroy();

	inline ui8* Get(OPuint index) {
		return (ui8*)items + index * _elementSize;
	}

	inline ui8* Front() {
		return Get(0);
	}

	inline ui8* Back() {
		return Get(_size - 1);
	}

	inline OPint IsEmpty() {
		return _size == 0;
	}

	inline OPint Size() {
		return _size;
	}

	inline OPint Capacity() {
		return _capacity;
	}

	inline void Clear() {
		_size = 0;
	}

	inline void Set(OPuint index, ui8* item) {
		OPmemcpy((ui8*)(items) + index * _elementSize, item, _elementSize);
	}

	inline void Erase(OPuint index) {
		EraseRange(index, index + 1);
	}

	inline void Push(ui8* item) {
		Insert(_size, item);
	}

	inline void Pop(ui8* item) {
		_size--;
	}
	
	inline static OPvector* Create(OPuint elementSize) { return OPNEW(OPvector(elementSize)); }
};
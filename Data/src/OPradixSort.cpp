#include "./Data/include/OPradixSort.h"

i64 OPradixSortGetMax(void* data, i64 n, i64(*getKey)(void*, i64)) {
	i64 mx = getKey(data, 0);
	for (i64 i = 1; i < n; i++) {
		i64 tmp = getKey(data, i);
		if (tmp > mx) {
			mx = tmp;
		}
	}
	return mx;
}

void OPradixSortCount(void* data, void* output, i64 n, i64 exp, i64(*getKey)(void*, i64), void*(*getEntity)(void*, i64), void(*setKey)(void*, i64, void*)) {
	void* entity;
	i64 i, key, count[10] = { 0 };

	for (i = 0; i < n; i++) {
		key = getKey(data, i);
		count[(key / exp) % 10]++;
	}

	for (i = 1; i < 10; i++) {
		count[i] += count[i - 1];
	}

	for (i = n - 1; i >= 0; i--) {
		key = getKey(data, i);
		entity = getEntity(data, i);
		setKey(output, count[(key / exp) % 10] - 1, entity);
		count[(key / exp) % 10]--;
	}

	for (i = 0; i < n; i++) {

		setKey(data, i, getEntity(output, i));
	}
}

void OPradixSort(void* data, void* duplicate, i64 n, i64(*getKey)(void*, i64), void*(*getEntity)(void*, i64), void(*setKey)(void*, i64, void*)) {
	i64 m = OPradixSortGetMax(data, n, getKey);

	for (i64 exp = 1; m / exp > 0; exp *= 10) {
		OPradixSortCount(data, duplicate, n, exp, getKey, getEntity, setKey);
	}
}
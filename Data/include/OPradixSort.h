#pragma once

#include "./Core/include/OPtypes.h"

/**
* Sorts an array based on a Radix sort algorithm
* @param data An array of data that contains a ui64 key
* @param duplicate An array of the same data and count as the data array
* @param n The number of elements in the data array
* @param getKey A function pointer to get the ui64 key out of the data array
* @param getEntity A function pointer to get the entity, for pulling into the duplicate
* @param setKey A function pointer to set the key in the data array
*/
void OPradixSort(void* data, void* duplicate, i64 n, i64(*getKey)(void*, i64), void*(*getEntity)(void*, i64), void(*setKey)(void*, i64, void*));
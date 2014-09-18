#pragma once

#include "./Core/include/Types.h"

#include <iostream>
#include <fstream>
using namespace std;

void write(ofstream* stream, void* data, i32 size) {
	stream->write((char*)data, size);
}
void writeI8(ofstream* stream, i8 val) {
	write(stream, &val, sizeof(i8));
}
void writeF32(ofstream* stream, f32 val) {
	write(stream, &val, sizeof(f32));
}
void writeI16(ofstream* stream, i16 val) {
	write(stream, &val, sizeof(i16));
}
void writeU16(ofstream* stream, ui16 val) {
	write(stream, &val, sizeof(ui16));
}
void writeI32(ofstream* stream, i32 val) {
	write(stream, &val, sizeof(i32));
}
void writeU32(ofstream* stream, ui32 val) {
	write(stream, &val, sizeof(ui32));
}
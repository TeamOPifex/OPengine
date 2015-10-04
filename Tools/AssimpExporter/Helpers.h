#pragma once

#include "./Core/include/OPtypes.h"
#include "./Math/include/OPvec2.h"
#include "./Math/include/OPvec3.h"
#include "./Data/include/OPlist.h"
#include "./Core/include/OPlog.h"

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

OPint IsParam(char** argv, ui16 pos, i8* arg) {
	return strcmp(arg, argv[pos]) == 0;
}

i8* GetParameter(char** argv, ui16 argp, i8* arg, i8* shrt) {

	if (0 == strcmp(arg, argv[argp]) || 0 == strcmp(shrt, argv[argp]))
	{
		++arg;
		return argv[argp];
	}

	return NULL;
}

void print_help()
{
	OPlog("Usage: FBXtoOPM [--help]");
}
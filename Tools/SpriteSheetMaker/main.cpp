#include "./Core/include/Types.h"
#include "./Data/include/File.h"
#define WIN32_LEAN_AND_MEAN 0
#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include "tinydir.h"
#include "./Human/include/Utilities/ImagePNG.h"

#include <iostream>
#include <fstream>
using namespace std;

void print_help() {

}

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
void writeI32(ofstream* stream, i32 val) {
	write(stream, &val, sizeof(i32));
}
void writeUI32(ofstream* stream, ui32 val) {
	write(stream, &val, sizeof(ui32));
}

int main(int argc, char **argv) {
	int arg;

	i8* filename = NULL;
	i32 width = 0, height = 0, frameCount = 0;
	
	for (arg = 1; arg < argc; ++arg)
	{
		if (0 == strcmp("--help", argv[arg]) || 0 == strcmp("-?", argv[arg]))
		{
			print_help();
			exit(1);
		}

		if (0 == strcmp("--file", argv[arg]) || 0 == strcmp("-f", argv[arg]))
		{
			++arg;

			if (filename)
			{
				fprintf(stderr, "Multiple --file parameters.\n");
				print_help();
				exit(1);
			}

			if (arg >= argc)
			{
				fprintf(stderr, "No file given.\n");
				print_help();
				exit(1);
			}

			filename = argv[arg];
			continue;
		}

		if (0 == strcmp("--width", argv[arg]) || 0 == strcmp("-w", argv[arg]))
		{
			++arg;

			if (0 != width)
			{
				fprintf(stderr, "Multiple --width parameters.\n");
				print_help();
				exit(1);
			}

			if (arg >= argc)
			{
				fprintf(stderr, "No width given.\n");
				print_help();
				exit(1);
			}

			errno = 0;

			width = (OPint)atoi(argv[arg]);

			if (errno)
			{
				fprintf(stderr, "No valid width given.\n");
				print_help();
				exit(1);
			}

			continue;
		}

		if (0 == strcmp("--height", argv[arg]) || 0 == strcmp("-h", argv[arg]))
		{
			++arg;

			if (0 != height)
			{
				fprintf(stderr, "Multiple --height parameters.\n");
				print_help();
				exit(1);
			}

			if (arg >= argc)
			{
				fprintf(stderr, "No height given.\n");
				print_help();
				exit(1);
			}

			errno = 0;

			height = (OPint)atoi(argv[arg]);

			if (errno)
			{
				fprintf(stderr, "No valid height given.\n");
				print_help();
				exit(1);
			}

			continue;
		}

		if (0 == strcmp("--frames", argv[arg]))
		{
			++arg;

			if (0 != frameCount)
			{
				fprintf(stderr, "Multiple --frames parameters.\n");
				print_help();
				exit(1);
			}

			if (arg >= argc)
			{
				fprintf(stderr, "No frames given.\n");
				print_help();
				exit(1);
			}

			errno = 0;

			frameCount = (OPint)atoi(argv[arg]);

			if (errno)
			{
				fprintf(stderr, "No valid frames given.\n");
				print_help();
				exit(1);
			}

			continue;
		}
	}



	// width
	// height
	// sprite count
	// frame counts
	// Sprite Name
	// Flags
		// Foreach Frame
		// x,y,w,h


	ofstream myFile("output.opss", ios::binary);
	writeI32(&myFile, width); // width
	writeI32(&myFile, height); // height
	writeI32(&myFile, 1); // Sprite count
	writeI32(&myFile, frameCount); // Frame Count
	writeUI32(&myFile, 5);
	myFile.write("gripe", 5 * sizeof(char));
	writeI32(&myFile, 1); // Flags - Animation
	writeI32(&myFile, frameCount); // number of frames
	for (i32 i = 0; i < frameCount; i++) {
		writeI32(&myFile, i * width);
		writeI32(&myFile, 0);
		writeI32(&myFile, width);
		writeI32(&myFile, height);
	}

	OPstream* str = OPreadFileLarge(filename, 1024);	
	write(&myFile, str->Data, str->Length);

	myFile.close();

	return 0;
}
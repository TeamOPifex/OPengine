#include "./../Core/include/Core.h"
#include "include/OPstream.h"
#include "include/OPfile.h"
#include "include/OPbatch.h"
#include <stdio.h>

int main(){
	OPstream* stream = OPstreamCreate(sizeof(int) * 4);

	// write some shit in
	{
		OPint data[3] = { 1, 33, 7 };
		int i = 0;

		for(i = 0; i < 30; i ++)
			OPwrite(stream, &data, sizeof(OPint) * 3);
	}

	OPseek(stream, 0);

	// read some shit!
	{
		int i, *j, k;

		for(i = 0; i < 30; i++){
			j = (int*)OPread(stream, sizeof(int) * 3);

			for(k = 0; k < 3; k++){
				printf("%d ", j[k]);
			} printf("\n");
		}
	}

	// destroy the stream
	OPstreamDestroy(stream);

	{
		int i = 0;
		ui8 c = '\n';
		stream = (OPstream*)OPreadFile("./main.c");

		printf("Size: %d\n", stream->Length);

		printf("%s\n", stream->Data);

		OPstreamDestroy(stream);
		printf("Stream has been destroyed\n");
	}

	{
		OPbatch* batch = OPbatchCreate(100, sizeof(OPint));
		OPint i;

		for(i = 0; i < 100; i++){
			OPbatchSet(batch, i, &i);
		}

		for(i = 0; i < 100; i++){
			OPint* I = (OPint*)OPbatchGet(batch, i);
			printf("%d ", *I);
		}
	}
	return 0;
}

#include "./../Core/include/Core.h"
#include "include/OPstream.h"
#include "include/OPfile.h"
#include "include/OPbatch.h"
#include <stdio.h>

using namespace OPEngine::Data;

int main(){
	OPStream* stream = new OPStream(sizeof(int) * 4);//OPstreamCreate(sizeof(int) * 4);

	// write some shit in
	{
		OPint data[3] = { 1, 33, 7 };
		int i = 0;

		for(i = 0; i < 30; i ++)
			stream->Write(&data, sizeof(OPint) * 3);
	}

	stream->Seek(0);

	// read some shit!
	{
		int i, *j, k;

		for(i = 0; i < 30; i++){
			j = (int*)stream->Read(sizeof(int) * 3);

			for(k = 0; k < 3; k++){
				printf("%d ", j[k]);
			} printf("\n");
		}
	}

	// destroy the stream
	delete stream;

	{
		int i = 0;
		ui8 c = '\n';
		stream = OPFile::Read("./main.c");

		if(stream){
			printf("Size: %d\n", stream->Length());

			printf("%s\n", stream->Data());

			delete stream;
			printf("Stream has been destroyed\n");
		}
	}
	OPBatch* batch = new OPBatch(100, sizeof(OPint));

	for(int i = 0; i < 100; i++) batch[i] = i;

	delete batch;

	return 0;
}

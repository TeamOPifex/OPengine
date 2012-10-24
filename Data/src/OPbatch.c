#include "./../include/OPbatch.h"

//-----------------------------------------------------------------------------
OPbatch* OPbatchCreate(OPuint elements, OPuint elementSize){
	OPbatch* batch = (OPbatch*)OPalloc(sizeof(OPbatch));

	if(batch){
		if(batch->Data = OPalloc(elementSize * elements)){
			batch->ElementSize = elementSize;
			batch->Elements = elements;
		}
		else{
			printf("Batch allocation failed\n");
			OPfree(batch);
			batch = NULL;
		}
	}
	else
		batch = NULL;
	return batch;
}
//-----------------------------------------------------------------------------
void* OPbatchGet(OPbatch* batch, OPuint index){
	ui8* D = (ui8*)batch->Data;
	return (void*)(D + (batch->ElementSize * index));
}
//-----------------------------------------------------------------------------
void OPbatchSet(OPbatch* batch, OPuint index, void* element){
	OPuint elementSize = batch->ElementSize;
	ui8* D = (ui8*)batch->Data;
	ui8* E = (ui8*)element;
	OPint* in = (OPint*)element, i;

	D += elementSize * index;

	printf("=%d ", *in);

	for(i = 0; i < elementSize; i++)
		*(D + i) = E[i];
}
//-----------------------------------------------------------------------------
OPint OPbatchDestroy(OPbatch* batch){
	if(batch){
		OPfree(batch->Data);
		OPfree(batch);
		return 1;
	}
	else
		return -1;
}
//-----------------------------------------------------------------------------
#ifdef __cplusplus
using namespace OPEngine::Data;

OPBatch::OPBatch(OPuint elements, OPuint elementSize){
	this->_batch = OPbatchCreate(elements, elementSize);
}
OPBatch::~OPBatch(){
	OPbatchDestroy(this->_batch);
}
void OPBatch::Set(OPuint index, void* element){
	OPbatchSet(this->_batch, index, element);
}

void* OPBatch::Get(OPuint index){
	OPbatch* b = this->_batch;
	return OPbatchGet(b, index);
}

ui8* OPBatch::operator[](OPuint index){
	return (ui8*)this->Get(index);
}

#endif



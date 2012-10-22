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

#endif



#pragma once
#ifndef OPEngine_Data_Batch
#define OPEngine_Data_Batch
#include "./../../Core/include/Core.h"

typedef struct{
	void* Data;
	OPuint Elements;
	OPuint ElementSize;
} OPbatch;


// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif
//-----------------------------------------------------------------------------
OPbatch* OPbatchCreate(OPuint elements, OPuint elementSize);
//-----------------------------------------------------------------------------
void* OPbatchGet(OPbatch* batch, OPuint index);
//-----------------------------------------------------------------------------
OPint OPbatchDestroy(OPbatch* batch);
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}

namespace OPEngine{
namespace Data{
	class OPBatch{
		public:
			OPBatch(OPuint elements, OPuint elementSize);
			~OPBatch();
			void* operator[](OPuint index){
				ui8* D = (ui8*)this->_batch->Data;
				OPuint eleSize = this->_batch->ElementSize;
				return (void*)(D + (index * eleSize));
			}
		private:
			OPbatch* _batch;
	};
}
}
#endif

#endif



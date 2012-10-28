#pragma once
#ifndef OPEngine_Data_Batch
#define OPEngine_Data_Batch
#include <stdio.h>
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
void OPbatchSet(OPbatch* batch, OPuint index, void* element);
//-----------------------------------------------------------------------------
OPint OPbatchDestroy(OPbatch* batch);
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}

namespace OPEngine{
namespace Data{
	class OPBatchElement{
		public:
			OPBatchElement(void* data, OPuint size){
				_size = size;
				Data = data;
			}
			OPBatchElement& operator=(void* element){
				ui8* D = (ui8*)Data;
				ui8* E = (ui8*)element;
				
				for(OPint i = 0; i < _size; D[i] = E[i++]);

				return *this;
			}
			void* Data;
		private:
			OPuint _size;
	};

	class OPBatch{
		public:
			OPBatch(OPuint elements, OPuint elementSize);
			~OPBatch();
			OPBatchElement operator[](OPuint index);
			OPBatch& operator=(void* element);
			void Set(OPuint index, void* element);
			OPBatchElement Get(OPuint index);
		private:
			OPbatch* _batch;
			OPuint _lastIndex;
	};
}
}
#endif

#endif



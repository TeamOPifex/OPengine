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
	class OPBatch{
		public:
			OPBatch(OPuint elements, OPuint elementSize);
			~OPBatch();
			ui8* operator[](OPuint index);
			void Set(OPuint index, void* element);
			void* Get(OPuint index);
		private:
			OPbatch* _batch;
	};
}
}
#endif

#endif



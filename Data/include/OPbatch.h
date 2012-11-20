#pragma once
#ifndef OPEngine_Data_Batch
#define OPEngine_Data_Batch
#include <stdio.h>

#if defined(OPIFEX_ANDROID) && defined(__cplusplus)
#include "./../../Core/include/Target.h"
#include "./../../Core/include/Types.h"
#include "./../../Core/include/DynamicMemory.h"
#else
#include "./../../Core/include/Core.h"
#endif

/**
 * Data container, designed to hold data in contiguous sections.
 *	OPbatch struct is responsible for holding generic data. The elements
 *	in a batch are all the same size in bytes. Batches cannot grow
 *	instead their entire capacity is allocated at creation.
 */
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
/**
 * OPbatchCreate - creates an OPbatch instance.
 *	Allocates space and initializes an OPbatch struct, then returns a
 *	pointer to the allocated memory segment. If allocation fails, a NULL
 *	pointer is returned.
 *	@param elements Number of objects which the batch can contain.
 *	@param elementSize The size in bytes of a single element.
 *	@return Pointer to an OPbatch instance.
 */
OPbatch* OPbatchCreate(OPuint elements, OPuint elementSize);
//-----------------------------------------------------------------------------
/**
 * OPbatchGet - retrieves an element from a batch
 *	Extracts an element from the provided batch at a specific index. This
 *	function does not check the batch pointer before operating on it.
 *	@param batch Pointer to a OPbatch struct.
 *	@param index Element in the batch whos pointer will be returned.
 *	@return Pointer an element in the batch.  
 */
void* OPbatchGet(OPbatch* batch, OPuint index);
//-----------------------------------------------------------------------------
/**
 * OPbatchSet - inserts an element into a batch
 *	Inserts an element into the specified batch at a specific index. This
 *	function does not check the batch pointer before operating on it.
 *	@param batch Pointer to a OPbatch struct.
 *	@param index Element in the batch whos pointer will be returned.
 *	@param pointer to the element being inserted.
 *	@return Pointer an element in the batch.  
 */
void OPbatchSet(OPbatch* batch, OPuint index, void* element);
//-----------------------------------------------------------------------------
/**
 * OPbatchDestroy - destroys an OPbatch instance
 *	Frees memory occupied by an OPbatch instance. Does nothing if a null
 *	pointer is passed.
 * @param batch Pointer to an OPbatch instance which will be destroyed.
 */
OPint OPbatchDestroy(OPbatch* batch);
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}

namespace OPEngine{
namespace Data{
	/**
	 * OPBatchElement - Wrapper class for batch data.
	 *	In order to provide the OPBatch class with correctly
	 *	functioning operator overrides (specifically for []
	 *	and = operators) data is contained within OPBatchElement
	 *	objects.
	 */
	class OPBatchElement{
		public:
			/**
			 * OPBatchElement - Constructor
			 *	@param data Pointer to the data to be contained
			 *	@param size Size in bytes of the element
			 */
			OPBatchElement(void* data, OPuint size){
				_size = size;
				Data = data;
			}
			/**
			 * Sets the contents of the OPBatchElement to the data
			 * pointed to by the argument
			 *	@param element Pointer to the data to be contained
			 *	@return pointer to this OPBatchElement object
			 */
			OPBatchElement& operator=(void* element){
				ui8* D = (ui8*)Data;
				ui8* E = (ui8*)element;
				
				for(OPint i = 0; i < _size; D[i] = E[i++]);

				return *this;
			}
			void* Data; //! Byte stream contents of the element
		private:
			OPuint _size; //! Size of the element in bytes
	};

	/**
	 * OPBatch - Object representation of an OPbatch.
	 *	This class allows for the construction of OPBatch objects
	 *	which allow for the use of a batch in a more convinient 
	 *	self contained manner.
	 */
	class OPBatch{
		public:
			/**
			 * OPBatch - Constructor.
			 *	@param elements Number of elements which this batch can hold
			 *	@param elementSize Size in bytes of the elements
			 */
			OPBatch(OPuint elements, OPuint elementSize);
			//! Destructor
			~OPBatch();
			
			OPBatchElement operator[](OPuint index);
			OPBatch& operator=(void* element);

			/**
			 * OPbatch::Set - inserts an element into the batch
			 *	Inserts an element into the specified batch at a specific index.
			 *	@param index Element in the batch whos pointer will be returned.
			 *	@param pointer to the element being inserted.  
			 */
			void Set(OPuint index, void* element);
			/**
			 * OPbatchGet - retrieves an element from a batch
			 *	Extracts an element from the provided batch at a specific index. This
			 *	function does not check the batch pointer before operating on it.
			 *	@param index Element in the batch whos pointer will be returned.
			 *	@return OPBatchElement object, a container for the data.  
			 */
			OPBatchElement Get(OPuint index);
		private:
			OPbatch* _batch;
			OPuint _lastIndex;
	};
}
}
#endif

#endif

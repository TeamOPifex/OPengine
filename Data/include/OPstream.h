// TODO: Have option to use a FILE* to read from disk rather than memory
#pragma once

struct OPstream;
typedef struct OPstream OPstream;

#include "./Core/include/OPtypes.h"
#include "./Core/include/OPmemory.h"

struct OPstream {
	OPuint Length;   // bytes in this stream
	OPuint Size;
	i8 Buffer[128];
	OPuint _pointer; // current location to write to
	OPchar* Source;
	ui8* Data;

	OPstream() { }
	OPstream(OPuint size) { Init(size); }

	void Init(OPuint size);

	//-----------------------------------------------------------------------------
	/**
	* OPstreamCreate - Allocates a byte stream instance.
	*	@param size Initial capacity in bytes
	*	@return Pointer to new stream instance
	*/
	inline static OPstream* Create(OPuint size) { return OPNEW(OPstream(size)); }

	//-----------------------------------------------------------------------------
	/**
	* OPstreanDestroy - Deallocates memory used by a stream.
	*	@param stream Stream instance to deallocate
	*	@return 0 if stream is null. 1 if deallocation is successful.
	*/
	void Destroy();

	//-----------------------------------------------------------------------------
	/**
	* OPwrite - Append data to the stream.
	*	@param stream Pointer to the stream to write to
	*	@param data Pointer to the data to be written
	*	@param size Number of bytes being written
	*	@return 1 if successful, 0 if failure
	*/
	OPuint Write(void* data, OPuint size);
	inline OPuint Write(f32* data) { return Write(data, sizeof(f32)); }

	//-----------------------------------------------------------------------------
	/**
	* OPread - Reads data and advances the pointer.
	*	@param stream Pointer to the stream to read from
	*	@param size Number of bytes to be read
	*	@return Pointer to a set of bytes existing in the stream
	*          no allocation performed.
	*/
	ui8* Read(OPuint size);

	i8 I8();
	i16 I16();
	i32 I32();
	ui8 UI8();
	ui16 UI16();
	ui32 UI32();
	f32 F32();

	/* Reads the first string it finds in the stream
	* Defined by a separated space.
	* Ex: 'Apple Banana Pear' would return Apple
	* @param stream The OPstream to read from
	* @return An allocated char* containing the string found (null terminated)
	*/
	OPchar* String();

	/* Reads the next line available in the OPstream
	* Reads until it finds a \n and will remove all \r
	* @param stream The OPstream to read from
	* @return An allocated char* containing the line found (null terminated)
	*/
	OPchar* ReadLine();

	/* Reads a text key value pair
	* Ex: '  Test   = My Value  ' would return a KeyValue of
	*     Key : Test
	*     Value : My Value
	* @param stream The OPstream to read from
	* @param dst A pointer to a KeyValuePair (it will be filled on success)
	* @return Success Result
	*/
	OPint ReadKeyValuePair(struct OPkeyValuePair* dst);

	//-----------------------------------------------------------------------------
	/**
	* OPreadAt - Reads data at a specific location.
	*	@param stream Pointer to the stream to read from
	*  @param pos Index from which to start reading
	*	@param size Number of bytes to be read
	*	@return Pointer to a set of bytes existing in the stream
	*          no allocation performed.
	*/
	ui8* ReadAt(OPuint pos, OPuint size);

	//-----------------------------------------------------------------------------
	/**
	* OPcopy - Reads data, and copies it into a provided buffer
	*      @param stream Pointer to the stream to copy from
	*      @param dest Pointer to the buffer to which data will be copied
	*      @param size Number of bytes to copy from stream, to dest
	*      @return Greater than 0 on success, less than or equal to on failure
	*/
	OPuint Copy(void* dest, OPuint size);

	//-----------------------------------------------------------------------------
	/**
	* OPseek - Skip to a specific byte location in the stream.
	*	@param stream Pointer to the stream to seek
	*	@param byte Byte index to skip to.
	*	@return Returns 0 if not in bounds and 1 if Seeking was successful
	*/
	OPuint Seek(OPuint byte);
};
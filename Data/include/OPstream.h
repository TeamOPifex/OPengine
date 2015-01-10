// TODO: Have option to use a FILE* to read from disk rather than memory


#ifndef OPEngine_Data_Stream
#define OPEngine_Data_Stream

#include "./Core/include/OPtypes.h"
#include "./Core/include/OPmemory.h"


typedef struct{
	OPuint Length;   // bytes in this stream
	OPuint Size;
	i8 Buffer[128];
	OPuint _pointer; // current location to write to
	ui8* Data;
} OPstream;

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
/**
 * OPstreamCreate - Allocates a byte stream instance.
 *	@param size Initial capacity in bytes
 *	@return Pointer to new stream instance
 */
	EXPORT_METHOD OPstream* OPstreamCreate(OPuint size);

//-----------------------------------------------------------------------------
/**
 * OPstreanDestroy - Deallocates memory used by a stream.
 *	@param stream Stream instance to deallocate
 *	@return 0 if stream is null. 1 if deallocation is successful.
 */
	EXPORT_METHOD OPuint OPstreamDestroy(OPstream* stream);

//-----------------------------------------------------------------------------
/**
 * OPwrite - Append data to the stream.
 *	@param stream Pointer to the stream to write to
 *	@param data Pointer to the data to be written
 *	@param size Number of bytes being written
 *	@return 1 if successful, 0 if failure
 */
	EXPORT_METHOD OPuint OPwrite(OPstream* stream, void* data, OPuint size);

//-----------------------------------------------------------------------------
/**
 * OPread - Reads data and advances the pointer.
 *	@param stream Pointer to the stream to read from
 *	@param size Number of bytes to be read
 *	@return Pointer to a set of bytes existing in the stream
 *          no allocation performed.
 */
	EXPORT_METHOD ui8* OPread(OPstream* stream, OPuint size);

	EXPORT_METHOD i8 OPstreamI8(OPstream* stream);
	EXPORT_METHOD i16 OPstreamI16(OPstream* stream);
	EXPORT_METHOD i32 OPstreamI32(OPstream* stream);
	EXPORT_METHOD ui8 OPstreamUI8(OPstream* stream);
	EXPORT_METHOD ui16 OPstreamUI16(OPstream* stream);
	EXPORT_METHOD ui32 OPstreamUI32(OPstream* stream);
	EXPORT_METHOD f32 OPstreamf32(OPstream* stream);

/* Reads the first string it finds in the stream
 * Defined by a separated space.
 * Ex: 'Apple Banana Pear' would return Apple
 * @param stream The OPstream to read from
 * @return An allocated char* containing the string found (null terminated)
*/
	EXPORT_METHOD OPchar* OPstreamString(OPstream* stream);

/* Reads the next line available in the OPstream
 * Reads until it finds a \n and will remove all \r
 * @param stream The OPstream to read from
 * @return An allocated char* containing the line found (null terminated)
*/
	EXPORT_METHOD OPchar* OPstreamReadLine(OPstream* stream);

/* Reads a text key value pair
 * Ex: '  Test   = My Value  ' would return a KeyValue of
 *     Key : Test
 *     Value : My Value
 * @param stream The OPstream to read from
 * @param dst A pointer to a KeyValuePair (it will be filled on success)
 * @return Success Result
*/
	EXPORT_METHOD OPint OPstreamReadKeyValuePair(OPstream* stream, OPkeyValuePair* dst);

//-----------------------------------------------------------------------------
/**
 * OPreadAt - Reads data at a specific location.
 *	@param stream Pointer to the stream to read from
 *  @param pos Index from which to start reading
 *	@param size Number of bytes to be read
 *	@return Pointer to a set of bytes existing in the stream
 *          no allocation performed.
 */
	EXPORT_METHOD ui8* OPreadAt(OPstream* stream, OPuint pos, OPuint size);

//-----------------------------------------------------------------------------
/**
 * OPcopy - Reads data, and copies it into a provided buffer
 *      @param stream Pointer to the stream to copy from
 *      @param dest Pointer to the buffer to which data will be copied
 *      @param size Number of bytes to copy from stream, to dest
 *      @return Greater than 0 on success, less than or equal to on failure
 */
	EXPORT_METHOD OPuint OPcopy(OPstream* stream, void* dest, OPuint size);

//-----------------------------------------------------------------------------
/**
 * OPseek - Skip to a specific byte location in the stream.
 *	@param stream Pointer to the stream to seek
 *	@param byte Byte index to skip to.
 *	@return Returns 0 if not in bounds and 1 if Seeking was successful
 */
	EXPORT_METHOD OPuint OPseek(OPstream* stream, OPuint byte);

#ifdef __cplusplus
};
//-----------------------------------------------------------------------------
//- C++ Definitions -----------------------------------------------------------
namespace OPEngine{
namespace Data{
	/**
	 * OPStream - Class which represents a byte stream.
	 *	An OPStream object contains a byte stream of dynamic size.
	 *	As more data is written to a stream, the stream will grow
	 *	to accomodate the new data.
	 */
	class OPStream{
		public:
			/**
			 * OPStream - Constructor
			 *	@param size Initial size in bytes of the stream.
			 */
			OPStream(OPuint size);
			~OPStream();
			/**
			 * Seek - Skip to a specific byte location in the stream.
			 *	@param byte Byte index to skip to.
			 *	@return Returns 0 if not in bounds and 1 if
			 *	        Seeking was successful
			 */
			OPuint Seek(OPuint byte);
			/**
			 * Write - Append data to the stream.
			 *	@param data Pointer to the data to be written
			 *	@param size Number of bytes being written
			 *	@return 1 if successful, 0 if failure
			 */
			OPuint Write(void* data, OPuint size);
			/**
			 * Read - Reads data and advances the pointer.
			 *	@param size Number of bytes to be read
			 *	@return Pointer to a set of bytes
			 */
			ui8*   Read(OPuint size);
			/**
			 * Data - Returns a pointer to the entire data stream.
			 *	@return Pointer to the data stream.
			 */
			ui8*   Data(){ return this->_stream->Data; }
			/**
			 * Length - Size of the stream.
			 *	@return Size in bytes of the entire stream.
			 */
			OPuint Length(){ return this->_stream->_pointer; }
			/**
			 * GetStream - Getter for the internal OPstream.
			 *	@return pointer to the internal OPstream
			 */
			OPstream* GetStream(){ return _stream; }
		private:
			OPstream* _stream;
	};
}
}
#endif
#endif

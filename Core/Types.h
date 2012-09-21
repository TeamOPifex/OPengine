#ifndef OPEngine_Core_Types
#define OPEngine_Core_Types

// define mathematical types

// integer types
typedef signed long    i64;
typedef unsigned long  ui64;  //long type
typedef signed int     i32;
typedef unsigned int   ui32; //int type
typedef signed short   i16;
typedef unsigned short ui16; //short type
typedef signed char    i8;
typedef unsigned char  ui8;  //byte type

// pointer
typedef unsigned int iptr;

// cross platform
typedef int OPint;

// floating point types
typedef float  f32; //float type
typedef double d64; //double type

// string types
typedef struct{
	ui8* Data;
	ui32 Length;
} OPstring;

#if defined(OPIFEX_ANDROID) || defined(OPIFEX_WIN32) || defined(OPIFEX_LINUX32)
typedef i32 sint;
typedef ui32 uint;
#elif defined(OPIFEX_WIN64) || defined(OPIFEX_LINUX64)
typedef i64 sint;
typedef ui64 uint;
#else
// default to building 32 bit binaries
typedef i32 sint;
typedef ui32 uint; 
#endif

#endif

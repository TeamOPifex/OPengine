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

// floating point types
typedef float  f32; //float type
typedef double d64; //double type

// string types
typedef struct{
	ui8* Data;
	ui32 Length;
} OPstring;

#endif

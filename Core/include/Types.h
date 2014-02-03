#pragma once
#ifndef OPEngine_Core_Types
#define OPEngine_Core_Types

#if defined(OPIFEX_ANDROID)
#include <jni.h>
#endif

// define mathematical types

#if defined(OPIFEX_ANDROID)
// android integer types
typedef long                i64;
typedef unsigned long       ui64;  //long type
typedef int                 i32;
typedef unsigned int        ui32; //int type
typedef jshort              i16;
typedef jchar               ui16; //short type
typedef jbyte               i8;
typedef jboolean            ui8;  //byte type

// android floating point types
typedef jfloat  f32; //float type
typedef jdouble d64; //double type
#else

// PC integer types
#if defined(OPIFEX_WIN64) || defined(OPIFEX_LINUX64)
typedef long    i64;
typedef unsigned long  ui64;  //long type
typedef int     i32;
typedef unsigned int   ui32; //int type
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_LINUX32) || defined(OPIFEX_OSX32)
typedef long    i64;
typedef unsigned long  ui64;  //long type
typedef int     i32;
typedef unsigned int   ui32; //int type
#endif

typedef short   i16;
typedef unsigned short ui16; //short type
typedef char    i8;
typedef unsigned char  ui8;  //byte type

// PC floating point types
typedef float  f32; //float type
typedef double d64; //double type

#endif

// Define cross platform base types
typedef char OPchar;
#if defined(OPIFEX_ANDROID) || defined(OPIFEX_WIN32) || defined(OPIFEX_LINUX32) || defined(OPIFEX_OSX32)
typedef i32 OPint;
typedef ui32 OPuint;
typedef f32 OPfloat;
#elif defined(OPIFEX_WIN64) || defined(OPIFEX_LINUX64) || defined(OPIFEX_OSX64)
typedef i64 OPint;
typedef ui64 OPuint;
typedef f32 OPfloat;
#else
// default to building 32 bit binaries
typedef i32 OPint;
typedef ui32 OPuint;
typedef f32 OPfloat;
#endif

// string types
typedef struct{
	ui8* Data;
	OPuint Length;
} OPstring;

#endif

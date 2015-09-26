#ifndef OPENGINE_CORE_TYPES
#define OPENGINE_CORE_TYPES

#if defined(OPIFEX_OSX64) || defined(OPIFEX_LINUX64) || defined(OPIFEX_OSX32) || defined(OPIFEX_LINUX32) || defined(OPIFEX_ANDROID) || defined(OPIFEX_IOS)
	#define OPIFEX_UNIX
#endif

#if defined(OPIFEX_LINUX64) || defined(OPIFEX_LINUX32)
	#define OPIFEX_LINUX
#endif

#if defined(OPIFEX_OSX64) || defined(OPIFEX_OSX32)
	#define OPIFEX_OSX
#endif

#if defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
	#define OPIFEX_WINDOWS
#endif

#if defined(OPIFEX_OSX64) || defined(OPIFEX_LINUX64) || defined(OPIFEX_WIN64) || defined(OPIFEX_IOS)
#define OPIFEX_OS64
#else
#define OPIFEX_OS32
#endif


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
#if defined(OPIFEX_OS64)
	typedef long long   i64;
	#ifdef OPIFEX_WINDOWS
		typedef unsigned long long  ui64;  //long type
	#elif defined(OPIFEX_UNIX)
		typedef unsigned long  ui64;  //long type
	#endif
	typedef int     i32;
	typedef unsigned int   ui32; //int type
#elif defined(OPIFEX_OS32)
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

// Varies between platforms
// On Android it's of size of 16
// on Desktop it's of size of 8
typedef char OPchar;
#if defined(OPIFEX_OS32)
typedef i32 OPint;
typedef ui32 OPuint;
typedef f32 OPfloat;
#elif defined(OPIFEX_OS64)
typedef i64 OPint;
typedef ui64 OPuint;
typedef f32 OPfloat;
#else
// default to building 32 bit binaries

typedef i32 OPint;
typedef ui32 OPuint;
typedef f32 OPfloat;
#endif

// A standard Key Value Pair
// key[255], value[255]
typedef struct {
	OPchar key[255];
	OPchar value[255];
}OPkeyValuePair;

#endif

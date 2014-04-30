# - Try to find the Theora library
# Once done this will define
#
#  VP8_FOUND - system has theora
#  VP8_LIBRARY         - The Theora library

include (CheckLibraryExists)

message(STATUS "Looking in: ${PROJECT_SOURCE_DIR}")

if( "${OPIFEX_OS}" STREQUAL "OPIFEX_WIN32" )

	find_library(VP8_LIBRARY 
	   NAMES vpxmt vpxmtd
	   PATHS
	   "${PROJECT_SOURCE_DIR}/External/VP8/lib/win32"
	)
	
elseif( "${OPIFEX_OS}" STREQUAL "OPIFEX_WIN64" )

   find_library(VP8_LIBRARY 
      NAMES vpxmt vpxmtd
      PATHS
      "${PROJECT_SOURCE_DIR}/External/VP8/lib/win64"
   )
endif()

message(STATUS "VP8: ${VP8_LIBRARY}")


if (VP8_LIBRARY)
   set(VP8_FOUND TRUE)
endif (VP8_LIBRARY)


if (VP8_FOUND)
      message(STATUS "Found VP8: ${VP8_LIBRARY}")
endif (VP8_FOUND)

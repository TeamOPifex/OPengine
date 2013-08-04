#C:\_Repos\OPifex\OPifex.Hg\OPifex.Engine\External\zlib\lib\win32\zlibd.lib

include (CheckLibraryExists)

if( "${OPIFEX_OS}" STREQUAL "OPIFEX_WIN32" )
  find_library(ZLIB_LIBRARY
    NAMES zlibd
    PATHS
    "${PROJECT_SOURCE_DIR}/External/zlib/lib/win32"
  )

  message(STATUS "ZLIB_LIBRARY (${OPIFEX_OS})")
  message(STATUS "ZLIB_LIBRARY lib: ${ZLIB_LIBRARY}")
  message(STATUS "")


  if (NOT ZLIB_LIBRARY)
        message(FATAL_ERROR "Could NOT find ZLIB_LIBRARY")
  endif()
endif()
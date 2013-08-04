#C:\_Repos\OPifex\OPifex.Hg\OPifex.Engine\External\libpng\lib\win32\libpng15d.lib

include (CheckLibraryExists)

if( "${OPIFEX_OS}" STREQUAL "OPIFEX_WIN32" )
  find_library(LIBPNG_LIBRARY
    NAMES libpng15d
    PATHS
    "${PROJECT_SOURCE_DIR}/External/libpng/lib/win32/"
  )

  message(STATUS "LIBPNG_LIBRARY (${OPIFEX_OS})")
  message(STATUS "LIBPNG_LIBRARY lib: ${LIBPNG_LIBRARY}")
  message(STATUS "")


  if (NOT LIBPNG_LIBRARY)
        message(FATAL_ERROR "Could NOT find LIBPNG_LIBRARY")
  endif()
endif()
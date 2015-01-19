# - Try to find the OggVorbis libraries
# Once done this will define
#
#  OGGVORBIS_FOUND - system has OggVorbis
#  OGGVORBIS_VERSION - set either to 1 or 2
#  OGGVORBIS_INCLUDE_DIR - the OggVorbis include directory
#  OGGVORBIS_LIBRARIES - The libraries needed to use OggVorbis
#  OGG_LIBRARY         - The Ogg library
#  VORBIS_LIBRARY      - The Vorbis library
#  VORBISFILE_LIBRARY  - The VorbisFile library

# Copyright (c) 2006, Richard Laerkaeng, <richard@goteborg.utfors.se>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.


include (CheckLibraryExists)

find_path(VORBIS_INCLUDE_DIR vorbis/vorbisfile.h 
   PATHS 
   "${PROJECT_SOURCE_DIR}/External/Vorbis/include"
)

find_path(OGG_INCLUDE_DIR ogg/ogg.h
   PATHS
   "${PROJECT_SOURCE_DIR}/External/Ogg/include"
)

message(STATUS "Looking in: ${PROJECT_SOURCE_DIR}")
message(STATUS "Vorbis Include: ${VORBIS_INCLUDE_DIR}")
message(STATUS "Ogg Include: ${OGG_INCLUDE_DIR}")

if( "${OPIFEX_OS}" STREQUAL "OPIFEX_WIN32" )

	find_library(OGG_LIBRARY_RELEASE
	   NAMES ogg libogg libogg_static
	   PATHS
	   "${PROJECT_SOURCE_DIR}/External/Ogg/lib/release/win32"
	)

	find_library(VORBIS_LIBRARY_RELEASE
	  NAMES vorbis libvorbis libvorbis_static
	  PATHS
	  "${PROJECT_SOURCE_DIR}/External/Vorbis/lib/release/win32"
	)

	find_library(VORBISFILE_LIBRARY_RELEASE
	   NAMES vorbisfile vorbisfile_static libvorbisfile libvorbisfile_static
	  PATHS
	  "${PROJECT_SOURCE_DIR}/External/Vorbis/lib/release/win32"
	)

	find_library(OGG_LIBRARY 
	   NAMES ogg libogg
	   PATHS
	   "${PROJECT_SOURCE_DIR}/External/Ogg/lib/debug/win32"
	)

	find_library(VORBIS_LIBRARY 
	  NAMES vorbis libvorbis
	  PATHS
	  "${PROJECT_SOURCE_DIR}/External/Vorbis/lib/debug/win32"
	)

	find_library(VORBISFILE_LIBRARY 
	   NAMES vorbisfile libvorbisfile
	  PATHS
	  "${PROJECT_SOURCE_DIR}/External/Vorbis/lib/debug/win32"
	)

elseif( "${OPIFEX_OS}" STREQUAL "OPIFEX_WIN64" )

	find_library(OGG_LIBRARY_RELEASE
	   NAMES ogg libogg libogg_static
	   PATHS
	   "${PROJECT_SOURCE_DIR}/External/Ogg/lib/release/win64"
	)

	find_library(VORBIS_LIBRARY_RELEASE
	  NAMES vorbis libvorbis libvorbis_static
	  PATHS
	  "${PROJECT_SOURCE_DIR}/External/Vorbis/lib/release/win64"
	)

	find_library(VORBISFILE_LIBRARY_RELEASE
	   NAMES vorbisfile vorbisfile_static libvorbisfile libvorbisfile_static
	  PATHS
	  "${PROJECT_SOURCE_DIR}/External/Vorbis/lib/release/win64"
	)
	
   find_library(OGG_LIBRARY 
      NAMES ogg libogg
      PATHS
      "${PROJECT_SOURCE_DIR}/External/Ogg/lib/debug/win64"
   )
   
   find_library(VORBIS_LIBRARY 
     NAMES vorbis libvorbis
     PATHS
     "${PROJECT_SOURCE_DIR}/External/Vorbis/lib/debug/win64"
   )

   find_library(VORBISFILE_LIBRARY 
      NAMES vorbisfile libvorbisfile
     PATHS
     "${PROJECT_SOURCE_DIR}/External/Vorbis/lib/debug/win64"
   )
else()

	include (CheckLibraryExists)

	find_path(VORBIS_INCLUDE_DIR vorbis/vorbisfile.h)
	find_path(OGG_INCLUDE_DIR ogg/ogg.h)

	find_library(OGG_LIBRARY NAMES ogg)
	find_library(VORBIS_LIBRARY NAMES vorbis)
	find_library(VORBISFILE_LIBRARY NAMES vorbisfile)
	find_library(VORBISENC_LIBRARY NAMES vorbisenc)

endif()


mark_as_advanced(OGG_LIBRARY_RELEASE)
mark_as_advanced(VORBIS_LIBRARY_RELEASE)
mark_as_advanced(VORBISFILE_LIBRARY_RELEASE)
mark_as_advanced(OGG_LIBRARY)
mark_as_advanced(VORBIS_LIBRARY)
mark_as_advanced(VORBISFILE_LIBRARY)

message(STATUS "Ogg: ${OGG_LIBRARY}")

mark_as_advanced(VORBIS_INCLUDE_DIR OGG_INCLUDE_DIR
                 OGG_LIBRARY VORBIS_LIBRARY VORBISFILE_LIBRARY)


if (VORBIS_INCLUDE_DIR AND VORBIS_LIBRARY AND VORBISFILE_LIBRARY)
   set(OGGVORBIS_FOUND TRUE)

   set(OGGVORBIS_LIBRARIES ${OGG_LIBRARY} ${VORBIS_LIBRARY} ${VORBISFILE_LIBRARY})

   set(_CMAKE_REQUIRED_LIBRARIES_TMP ${CMAKE_REQUIRED_LIBRARIES})
   set(CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} ${OGGVORBIS_LIBRARIES})
   check_library_exists(vorbis vorbis_bitrate_addblock "" HAVE_LIBVORBISENC2)
   set(CMAKE_REQUIRED_LIBRARIES ${_CMAKE_REQUIRED_LIBRARIES_TMP})

   if (HAVE_LIBVORBISENC2)
      set (OGGVORBIS_VERSION 2)
   else (HAVE_LIBVORBISENC2)
      set (OGGVORBIS_VERSION 1)
   endif (HAVE_LIBVORBISENC2)

else (VORBIS_INCLUDE_DIR AND VORBIS_LIBRARY AND VORBISFILE_LIBRARY)
   set (OGGVORBIS_VERSION)
   set(OGGVORBIS_FOUND FALSE)
endif (VORBIS_INCLUDE_DIR AND VORBIS_LIBRARY AND VORBISFILE_LIBRARY)


if (OGGVORBIS_FOUND)
   if (NOT OggVorbis_FIND_QUIETLY)
      message(STATUS "Found OggVorbis: ${OGGVORBIS_LIBRARIES}")
   endif (NOT OggVorbis_FIND_QUIETLY)
else (OGGVORBIS_FOUND)
   if (OggVorbis_FIND_REQUIRED)
      message(FATAL_ERROR "Could NOT find OggVorbis libraries")
   endif (OggVorbis_FIND_REQUIRED)
   if (NOT OggVorbis_FIND_QUITELY)
      message(STATUS "Could NOT find OggVorbis libraries")
   endif (NOT OggVorbis_FIND_QUITELY)
endif (OGGVORBIS_FOUND)

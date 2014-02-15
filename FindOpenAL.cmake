include (CheckLibraryExists)


if( ${OPIFEX_OS_WINDOWS} )
  find_path(OPENAL_INCLUDE_DIR al.h
    HINTS
      ENV OPENALDIR
    PATH_SUFFIXES include/AL include/OpenAL include
    PATHS
      "C:/Program Files (x86)/OpenAL 1.1 SDK"
    )
else()
  find_path(OPENAL_INCLUDE_DIR al.h
    HINTS
      ENV OPENALDIR
    PATH_SUFFIXES include/AL include/OpenAL include
    PATHS
    "C:/Program Files (x86)/OpenAL 1.1 SDK"
    ~/Library/Frameworks
    /Library/Frameworks
    /sw # Fink
    /opt/local # DarwinPorts
    /opt/csw # Blastwave
    /opt
    [HKEY_LOCAL_MACHINE\\SOFTWARE\\Creative\ Labs\\OpenAL\ 1.1\ Software\ Development\ Kit\\1.00.0000;InstallDir]
  )
endif()


if(  ${OPIFEX_OS_WIN32}  )
  find_library(OPENAL_LIBRARY
    NAMES OpenAL32
    HINTS
      ENV OPENALDIR
    PATH_SUFFIXES libs/Win32
    PATHS
    "C:/Program Files (x86)/OpenAL 1.1 SDK"
  )

elseif( ${OPIFEX_OS_WIN64} )
  message(STATUS "64 Bit OpenAL")
  find_library(OPENAL_LIBRARY
    NAMES OpenAL32
    HINTS
      ENV OPENALDIR
    PATH_SUFFIXES libs/Win64
    PATHS
    "C:/Program Files (x86)/OpenAL 1.1 SDK"
  )
endif()

message(STATUS "OpenAL include: ${OPENAL_INCLUDE_DIR}")
message(STATUS "OpenAL lib: ${OPENAL_LIBRARY}")
message(STATUS "")

if (NOT OPENAL_INCLUDE_DIR)
      message(FATAL_ERROR "Could NOT find OPENAL_INCLUDE_DIR")
endif()

if (NOT OPENAL_LIBRARY)
      message(FATAL_ERROR "Could NOT find OPENAL_LIBRARY")
endif()

mark_as_advanced(OPENAL_LIBRARY OPENAL_INCLUDE_DIR)

SET ( CMAKE_C_FLAGS "-stdlib=libstdc++" CACHE STRING "compile flags" FORCE)
SET ( CMAKE_CXX_FLAGS "-stdlib=libstdc++" CACHE STRING "compile flags" FORCE)

### CMake Setup
include(${CMAKE_CURRENT_LIST_DIR}/components.cmake) 
include(${CMAKE_CURRENT_LIST_DIR}/macros.cmake) 


if(OPIFEX_BINARIES)
	set_binary_output(OPIFEX_BINARIES_ABS)
	#get_filename_component(OPIFEX_BINARIES_ABS ${OPIFEX_BINARIES_ABS} REALPATH)
	include(${OPIFEX_BINARIES_ABS}/OPengineDefines.cmake)
endif()


### Build Setup
include(${CMAKE_CURRENT_LIST_DIR}/build.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/global.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/packages.cmake)

set_binary_output(BINARY_FOLDER)

mark_as_advanced(CMAKE_CONFIGURATION_TYPES)
mark_as_advanced(CMAKE_INSTALL_PREFIX)
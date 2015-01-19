
### CMake Setup
include(${CMAKE_CURRENT_LIST_DIR}/components.cmake) 
include(${CMAKE_CURRENT_LIST_DIR}/macros.cmake) 
### Build Setup
include(${CMAKE_CURRENT_LIST_DIR}/build.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/global.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/packages.cmake)


mark_as_advanced(CMAKE_CONFIGURATION_TYPES)
mark_as_advanced(CMAKE_INSTALL_PREFIX)
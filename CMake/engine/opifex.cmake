
### This is the entry point for the OPengine CMake build process

### CMake Setup
include(${CMAKE_CURRENT_LIST_DIR}/components.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/macros.cmake)


### Build Setup
include(${CMAKE_CURRENT_LIST_DIR}/build.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/find_packages.cmake)




### C/C++ Build Flags
if(${OPIFEX_OS_IOS})
	SET ( CMAKE_C_FLAGS "-stdlib=libstdc++ -fobjc-abi-version=2 -fobjc-arc -isysroot ${CMAKE_OSX_SYSROOT}" CACHE STRING "compile flags" FORCE)
	SET ( CMAKE_CXX_FLAGS "-stdlib=libstdc++ -std=c++11 -fobjc-abi-version=2 -fobjc-arc -std=gnu++11 -stdlib=libc++ -isysroot ${CMAKE_OSX_SYSROOT}" CACHE STRING "compile flags" FORCE)
	# -mios-simulator-version-min=6.0
else()
	SET ( CMAKE_C_FLAGS "-stdlib=libstdc++" CACHE STRING "compile flags" FORCE)
	SET ( CMAKE_CXX_FLAGS "-stdlib=libstdc++ -std=c++11" CACHE STRING "compile flags" FORCE)
endif()



### Output location for binaries
set_binary_output(BINARY_FOLDER)


### CleanUp
mark_as_advanced(CMAKE_CONFIGURATION_TYPES)
mark_as_advanced(CMAKE_INSTALL_PREFIX)

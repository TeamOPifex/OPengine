cmake_minimum_required(VERSION 2.8)

configure_file (
  "${PROJECT_SOURCE_DIR}/Core/include/Target.h.in"
  "${PROJECT_SOURCE_DIR}/Core/include/Target.h"
)

add_source_group(\\ "${PROJECT_SOURCE_DIR}/Core" CORE_SRC)
add_source_group(include/ "${PROJECT_SOURCE_DIR}/Core/include" CORE_SRC)
add_source_group(src/ "${PROJECT_SOURCE_DIR}/Core/src" CORE_SRC)


	
add_library(1_Core STATIC ${CORE_SRC})

if( "${OPIFEX_OS}" STREQUAL "OPIFEX_WIN32" )

add_custom_command(TARGET 1_Core POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${PROJECT_BINARY_DIR}/Core/Debug/Core.lib"
	${PROJECT_BINARY_DIR}/Binaries/win32/)
	
elseif("${OPIFEX_OS}" STREQUAL "OPIFEX_LINUX32")

add_custom_command(TARGET 1_Core POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${PROJECT_BINARY_DIR}/Core/libCore.a"
	${PROJECT_BINARY_DIR}/Binaries/linux32/)

	endif()
	
set_source_files_properties(src/test.c PROPERTIES LANGUAGE CXX )
SET_TARGET_PROPERTIES(1_Core PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(1_Core PROPERTIES OUTPUT_NAME "Core")

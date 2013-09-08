cmake_minimum_required(VERSION 2.8)

include_directories(
	.
	../
)

set(ALL_LIBS
	1_Core
	2_Data
)

add_definitions(
	-D_CRT_SECURE_NO_WARNINGS
)

add_source_group(include/ "${PROJECT_SOURCE_DIR}/Math/include" MATH_SRC)

add_source_group(src/ "${PROJECT_SOURCE_DIR}/Math/src" MATH_SRC)

add_library(3_Math STATIC ${MATH_SRC})
	
if( "${OPIFEX_OS}" STREQUAL "OPIFEX_WIN32" )
		
	add_custom_command(TARGET 3_Math POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_BINARY_DIR}/Math/Debug/Math.lib"
		${PROJECT_BINARY_DIR}/Binaries/win32/)
	
elseif("${OPIFEX_OS}" STREQUAL "OPIFEX_LINUX32")

	add_custom_command(TARGET 3_Math POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_BINARY_DIR}/Math/libMath.a"
		${PROJECT_BINARY_DIR}/Binaries/linux32/)

endif()
	
target_link_libraries(3_Math
	${ALL_LIBS}
)

SET_TARGET_PROPERTIES(3_Math PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(3_Math PROPERTIES 
	OUTPUT_NAME "Math"
)
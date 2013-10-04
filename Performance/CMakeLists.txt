cmake_minimum_required(VERSION 2.8)

include_directories(
	.
	../
)

set(ALL_LIBS
	1_Core
	2_Data
	3_Math
)

add_definitions(
	-D_CRT_SECURE_NO_WARNINGS
)

add_source_group(include/ "${PROJECT_SOURCE_DIR}/Performance/include" PERFORMANCE_SRC)

add_source_group(src/ "${PROJECT_SOURCE_DIR}/Performance/src" PERFORMANCE_SRC)


add_library(4_Performance STATIC ${PERFORMANCE_SRC})
	
if( "${OPIFEX_OS}" STREQUAL "OPIFEX_WIN32" )
		
	add_custom_command(TARGET 4_Performance POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_BINARY_DIR}/Math/Debug/Performance.lib"
		${PROJECT_BINARY_DIR}/Binaries/win32/)
	
elseif("${OPIFEX_OS}" STREQUAL "OPIFEX_LINUX32")

	add_custom_command(TARGET 4_Performance POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_BINARY_DIR}/Math/libPerformance.a"
		${PROJECT_BINARY_DIR}/Binaries/linux32/)

endif()


target_link_libraries(4_Performance
	${ALL_LIBS}
)


SET_TARGET_PROPERTIES(4_Performance PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(4_Performance PROPERTIES 
	OUTPUT_NAME "Performance"
)
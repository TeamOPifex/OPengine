cmake_minimum_required(VERSION 2.8)

include_directories(
	../External/glfw-2.7.6/include/
	../External/glm-0.9.1/
	../External/glew-1.5.8/include/
	../External/Phsyx.3.2.1/include/
	../External/GLES2/
	.
	../
)

set(ALL_LIBS
	1_Core
	2_Data
	4_Human
)


#if( "${OPIFEX_OS}" STREQUAL "OPIFEX_ANDROID" )
#add_source_group(\\ "${PROJECT_SOURCE_DIR}/Android" APPLICATION_SRC)
#elseif(1)
add_source_group(\\ "${PROJECT_SOURCE_DIR}/Application" APPLICATION_SRC)
#endif()

add_executable(7_Application ${APPLICATION_SRC})

target_link_libraries(7_Application
	${ALL_LIBS}
)

SET_TARGET_PROPERTIES(7_Application PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(7_Application PROPERTIES 
	OUTPUT_NAME "Application"
)
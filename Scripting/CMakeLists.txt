cmake_minimum_required(VERSION 2.8)

add_source_group(\\ "${PROJECT_SOURCE_DIR}/Scripting" SCRIPTING_SRC)

add_library(7_Scripting STATIC ${SCRIPTING_SRC})
SET_TARGET_PROPERTIES(7_Scripting PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(7_Scripting PROPERTIES 
	OUTPUT_NAME "Scripting"
)
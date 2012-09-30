cmake_minimum_required(VERSION 2.8)

configure_file (
  "${PROJECT_SOURCE_DIR}/Core/include/Target.h.in"
  "${PROJECT_SOURCE_DIR}/Core/include/Target.h"
)

add_source_group(\\ "${PROJECT_SOURCE_DIR}/Core" CORE_SRC)
add_source_group(include/ "${PROJECT_SOURCE_DIR}/Core/include" CORE_SRC)
add_source_group(src/ "${PROJECT_SOURCE_DIR}/Core/src" CORE_SRC)


add_library(1_Core STATIC ${CORE_SRC})
set_source_files_properties(test.c PROPERTIES LANGUAGE CXX )
SET_TARGET_PROPERTIES(1_Core PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(1_Core PROPERTIES OUTPUT_NAME "Core")

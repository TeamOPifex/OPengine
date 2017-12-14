#########################################
# CORE - LAYER 1
#########################################
cmake_minimum_required(VERSION 2.8)


#########################################
# Include Directories
#########################################
include_directories(
	.
	../
	./Core/include/
	./Core/include/Platform/
)
add_external_opifex_includes()

#########################################
# Include Files
#########################################
add_source_group(/ "${PROJECT_SOURCE_DIR}/Core" CORE_SRC)
add_source_group(include/ "${PROJECT_SOURCE_DIR}/Core/include" CORE_SRC)
add_source_group(include/Platform "${PROJECT_SOURCE_DIR}/Core/include/Platform" CORE_SRC)


#########################################
# Source Files
#########################################
add_source_group(src/ "${PROJECT_SOURCE_DIR}/Core/src" CORE_SRC)
add_source_group(src/Platform/ "${PROJECT_SOURCE_DIR}/Core/src/Platform" CORE_SRC)


#########################################
# BUILD
#########################################
if(${OPIFEX_OPTION_SHARED})
	add_library(1_Core SHARED ${CORE_SRC})
else()
	add_library(1_Core STATIC ${CORE_SRC})
endif()
if(${OPIFEX_OS_IOS})
	set_xcode_property(1_Core GCC_GENERATE_DEBUGGING_SYMBOLS YES)
endif()
add_opifex_defines()

if(${OPIFEX_OPTION_RELEASE})
	add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
endif()
set_target_properties(1_Core PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(1_Core PROPERTIES OUTPUT_NAME "Core")

if( ${OPIFEX_OS_WINDOWS} )
	target_link_libraries(1_Core Winmm.lib)
endif()

if( ${OPIFEX_OS_ANDROID} )
	#add_library(native_app_glue STATIC ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
	#target_include_directories(1_Core PRIVATE ${ANDROID_NDK}/sources/android/native_app_glue)
	#target_link_libraries(1_Core native_app_glue)
	target_link_libraries(1_Core log)
endif()

# install (TARGETS 1_Core DESTINATION ../../Binaries/ios/debug)
output_library(1_Core "Core")

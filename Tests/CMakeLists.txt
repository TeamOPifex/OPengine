########################################
# TESTS - LAYER
#########################################
cmake_minimum_required(VERSION 2.8)


#########################################
# Include Directories
#########################################
include_directories(
	.
	../
)

add_external_opifex_includes()

#########################################
# DEPENDENT LIBRARIES
#########################################
set(ALL_LIBS
	1_Core
	2_Data
	3_Math
	4_Performance
	5_Human
	7_Pipeline
)


# If necessary, use the RELATIVE flag, otherwise each source file may be listed
# with full pathname. RELATIVE may makes it easier to extract an executable name
# automatically.
# file( GLOB APP_SOURCES RELATIVE app/*.cxx )
# message(STATUS "About to build tests")
file( GLOB TEST_SOURCES ${PROJECT_SOURCE_DIR}/Tests/src/*.cpp )
foreach( testSourceFile ${TEST_SOURCES} )
	# I used a simple string replace, to cut off .cpp.
	string( REPLACE ".cpp" "" testName ${testSourceFile} )
	string( REPLACE "C:" "" testName ${testName} )
	string( REPLACE "Y:" "" testName ${testName} )
	string( REPLACE "Z:" "" testName ${testName} )
	string( REPLACE "D:" "" testName ${testName} )
	string( REGEX REPLACE "/.*/" "" testName ${testName} )
	add_executable( ${testName} ${testSourceFile} )
	set_property(TARGET ${testName} PROPERTY FOLDER "Tests")
	# message(STATUS "Library: ${testName}")
	add_definitions(-D${OPIFEX_OS})
	# Make sure the engine is linked to each test
	target_link_libraries( ${testName} ${ALL_LIBS} )
	#SET(CMAKE_EXE_LINKER_FLAGS, "-lm")
endforeach( testSourceFile ${TEST_SOURCES} )

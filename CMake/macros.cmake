
macro(add_source_group FILTER_NAME SOURCE_PATH TARGET_LIST)

	file(TO_NATIVE_PATH ${FILTER_NAME} NEW_FILTER_NAME)
	file(GLOB TEMP_SRC
		"${SOURCE_PATH}/*.h"
		"${SOURCE_PATH}/*.cpp"
		"${SOURCE_PATH}/*.c"
		"${SOURCE_PATH}/*.mm"
	)
	source_group("${NEW_FILTER_NAME}" FILES ${TEMP_SRC})
	list(APPEND ${TARGET_LIST} "${TEMP_SRC}")

endmacro(add_source_group)

macro(copy_source_group SOURCE_PATH DESTINATION_TARGET)

	message(STATUS "COPY FROM '${SOURCE_PATH}'' to '${DESTINATION_TARGET}'")
	file(GLOB TEMP_SRC "${SOURCE_PATH}/*")
	file(COPY ${TEMP_SRC} DESTINATION ${DESTINATION_TARGET}/)
	### INSTALL( DIRECTORY ${SOURCE_PATH} DESTINATION ${DESTINATION_TARGET} )

endmacro(copy_source_group)

macro(OPIFEX_EVAL name)
   IF(${ARGN})
     SET(${name} 1)
   ELSE(${ARGN})
     SET(${name} 0)
   ENDIF(${ARGN})
endmacro(OPIFEX_EVAL)

macro(populate_binary_directory)
	if("${OPIFEX_OS}" STREQUAL "OPIFEX_WIN32")
		SET(BINARY_TARGET_DIRECTORY "win32")
	elseif("${OPIFEX_OS}" STREQUAL "OPIFEX_WIN64")
		SET(BINARY_TARGET_DIRECTORY "win64")
	elseif("${OPIFEX_OS}" STREQUAL "OPIFEX_OSX32")
		SET(BINARY_TARGET_DIRECTORY "osx32")
	elseif("${OPIFEX_OS}" STREQUAL "OPIFEX_OSX64")
		SET(BINARY_TARGET_DIRECTORY "osx64")
	elseif("${OPIFEX_OS}" STREQUAL "OPIFEX_LINUX32")
		SET(BINARY_TARGET_DIRECTORY "linux32")
	elseif("${OPIFEX_OS}" STREQUAL "OPIFEX_LINUX64")
		SET(BINARY_TARGET_DIRECTORY "linux64")
	endif()
endmacro(populate_binary_directory)

macro(output_library APPLICATION_TARGET LIBRARY_NAME )

	if( "${OPIFEX_OS}" STREQUAL "${OPIFEX_ANDROID}" )

	else()
		populate_binary_directory()
		SET(COPY_BINARY_RELATIVE_DIRECTORY "/${LIBRARY_NAME}/")
		SET(COPY_BINARY_LIBRARY "lib${LIBRARY_NAME}.a")

		if("${OPIFEX_OS}" STREQUAL "OPIFEX_WIN32" OR "${OPIFEX_OS}" STREQUAL "OPIFEX_WIN64")
			SET(COPY_BINARY_RELATIVE_DIRECTORY "/${LIBRARY_NAME}/Debug/")
			SET(COPY_BINARY_LIBRARY "${LIBRARY_NAME}.lib")
		endif()

		add_custom_command(TARGET ${APPLICATION_TARGET} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${OPIFEX_PROJECT_BINARY_DIR}${COPY_BINARY_RELATIVE_DIRECTORY}${COPY_BINARY_LIBRARY}"
			${OPIFEX_PROJECT_BINARY_DIR}/Binaries/${BINARY_TARGET_DIRECTORY}/)
	endif()

endmacro(output_library)


macro(output_library_from APPLICATION_TARGET RELATIVE_PATH LIBRARY_NAME )

	if( "${OPIFEX_OS}" STREQUAL "${OPIFEX_ANDROID}" )

	else()
		populate_binary_directory()
		SET(COPY_BINARY_RELATIVE_DIRECTORY "${RELATIVE_PATH}/")
		SET(COPY_BINARY_LIBRARY "lib${LIBRARY_NAME}.a")

		if("${OPIFEX_OS}" STREQUAL "OPIFEX_WIN32" OR "${OPIFEX_OS}" STREQUAL "OPIFEX_WIN64")
			SET(COPY_BINARY_RELATIVE_DIRECTORY "${RELATIVE_PATH}/Debug/")
			SET(COPY_BINARY_LIBRARY "${LIBRARY_NAME}.lib")
		endif()

		add_custom_command(TARGET ${APPLICATION_TARGET} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${OPIFEX_PROJECT_BINARY_DIR}${COPY_BINARY_RELATIVE_DIRECTORY}${COPY_BINARY_LIBRARY}"
			${OPIFEX_PROJECT_BINARY_DIR}/Binaries/${BINARY_TARGET_DIRECTORY}/)
	endif()

endmacro(output_library_from)


macro(output_binary APPLICATION_TARGET RELATIVE_PATH FILE_PATH OPIFEX_MATCH )

	if( ${OPIFEX_MATCH} )
		populate_binary_directory()
		add_custom_command(TARGET ${APPLICATION_TARGET} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${OPIFEX_PROJECT_SOURCE_DIR}${RELATIVE_PATH}${BINARY_TARGET_DIRECTORY}/${FILE_PATH}"
			${OPIFEX_PROJECT_BINARY_DIR}/Binaries/${BINARY_TARGET_DIRECTORY}/)
	endif()

endmacro(output_binary)


macro(copy_to_folder APPLICATION_TARGET RELATIVE_PATH FILE_PATH OUTPUT_PATH OPIFEX_MATCH )

	
	if( ${OPIFEX_MATCH} )
		populate_binary_directory()
		add_custom_command(TARGET ${APPLICATION_TARGET} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${PROJECT_SOURCE_DIR}${RELATIVE_PATH}${BINARY_TARGET_DIRECTORY}/${FILE_PATH}"
			${PROJECT_BINARY_DIR}${OUTPUT_PATH})
			
	endif()

endmacro(copy_to_folder)

macro(copy_from_binaries_on_build APPLICATION_TARGET FILE_PATH OPIFEX_MATCH )

	
	if( ${OPIFEX_MATCH} )
		populate_binary_directory()
		add_custom_command(TARGET ${APPLICATION_TARGET} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${OPIFEX_BINARIES}/${BINARY_TARGET_DIRECTORY}/${FILE_PATH}"
			${PROJECT_BINARY_DIR}/Debug)
			
	endif()

endmacro(copy_from_binaries_on_build)

macro(add_opifex_includes)
	include_directories(
		${OPIFEX_ENGINE_REPOSITORY}
		${OPIFEX_ENGINE_REPOSITORY}/External/glfw-3.0.4/include/
		${OPIFEX_ENGINE_REPOSITORY}/External/glew-1.5.8/include/
		${OPIFEX_ENGINE_REPOSITORY}/External/OpenAL/
		${OPIFEX_ENGINE_REPOSITORY}/External/Ogg/include/
		${OPIFEX_ENGINE_REPOSITORY}/External/Vorbis/include/
		${OPIFEX_ENGINE_REPOSITORY}/External/glm-0.9.1/
	)
endmacro(add_opifex_includes)

macro(add_opifex_debug_definitions APPLICATION_DIR_DEPTH )

	add_definitions(-DGLEW_STATIC -D_CRT_SECURE_NO_WARNINGS -D${OPIFEX_OS})

	if(${OPIFEX_RELEASE})
		
	else()
		add_definitions(-D_DEBUG)
		if(${OPIFEX_OS_WINDOWS})
			SET(OPIFEX_REPO "${APPLICATION_DIR_DEPTH}${OPIFEX_REPOSITORY}/Assets/")
			add_definitions(-DOPIFEX_REPO="${OPIFEX_REPO}")
		else()
			SET(OPIFEX_REPO "${OPIFEX_REPOSITORY}/Assets/")
			add_definitions(-DOPIFEX_REPO="${OPIFEX_REPO}")
		endif()
	endif()

	if(${OPIFEX_MYO})
		add_definitions(-DOPIFEX_MYO)
	endif()
	
	set_target_properties(8_Application PROPERTIES LINKER_LANGUAGE CXX)

endmacro(add_opifex_debug_definitions)

macro(add_opifex_libraries APPLICATION_TARGET )
	find_binary(LIBLODEPNG "LodePng")
	find_binary(LIBCORE "Core")
	find_binary(LIBDATA "Data")
	find_binary(LIBMATH "Math")
	find_binary(LIBPERFORMANCE "Performance")
	find_binary(LIBSCRIPTING "Scripting")
	find_binary(LIBPIPELINE "Pipeline")
	find_binary(LIBHUMAN "Human")
	find_binary(LIBGLEW_158 "GLEW_158")
	find_binary(LIBGLFW "glfw3")

	target_link_libraries(${APPLICATION_TARGET}
		${OPENGL_LIBRARY}
		${LIBCORE}
		${LIBDATA}
		${LIBMATH}
		${LIBPERFORMANCE}
		${LIBLODEPNG}
		${LIBHUMAN}
		${LIBSCRIPTING}
		${LIBPIPELINE}
		${LIBGLEW_158}
		${LIBGLFW}
		${OPENAL_LIBRARY}
		${OpenGL}
	)

	if( ${OPIFEX_OS_WINDOWS} )
		find_binary(LIBOGG "libogg")
		find_binary(LIBVORBIS "libvorbis")
		find_binary(LIBVORBISFILE "libvorbisfile")
		target_link_libraries(${APPLICATION_TARGET}	
			Winmm.lib
			${LIBOGG}
			${LIBVORBIS}
			${LIBVORBISFILE}
		)
	endif()

	if( ${MSVC_WIN7_FIX} )
		target_link_libraries(${APPLICATION_TARGET}	Xinput9_1_0.lib)
	endif()

	if(${OPIFEX_OS_LINUX32} OR ${OPIFEX_OS_LINUX64})
		find_package( X11 REQUIRED )
		find_package( Threads )

		target_link_libraries(${APPLICATION_TARGET}
			${X11_LIBRARIES}
			${CMAKE_THREAD_LIBS_INIT}
			${CMAKE_DL_LIBS}
			${GLFW_LIBRARIES}
			${OGG_LIBRARY}
			${VORBIS_LIBRARY}
			${VORBISFILE_LIBRARY}
			-logg
		)
		SET(CMAKE_EXE_LINKER_FLAGS, "-ldl")
	endif()

	if(${OPIFEX_OS_OSX32} OR ${OPIFEX_OS_OSX64})
		
		target_link_libraries(${APPLICATION_TARGET}
			${CMAKE_THREAD_LIBS_INIT}
			${OGG_LIBRARY}
			${VORBIS_LIBRARY}
			${VORBISFILE_LIBRARY}
			${GLFW_LIBRARIES}
		)
		
	endif()
	
	
	copy_from_binaries_on_build(${APPLICATION_TARGET} "ogg.dll" ${OPIFEX_OS_WINDOWS})
	copy_from_binaries_on_build(${APPLICATION_TARGET} "libogg.dll" ${OPIFEX_OS_WINDOWS})
	copy_from_binaries_on_build(${APPLICATION_TARGET} "vorbisfile.dll" ${OPIFEX_OS_WINDOWS})
	copy_from_binaries_on_build(${APPLICATION_TARGET} "libvorbis.dll" ${OPIFEX_OS_WINDOWS})
	copy_from_binaries_on_build(${APPLICATION_TARGET} "libvorbisfile.dll" ${OPIFEX_OS_WINDOWS})
	#copy_from_binaries_on_build(${APPLICATION_TARGET} "myo32.dll" ${OPIFEX_OS_WINDOWS})
	#copy_from_binaries_on_build(${APPLICATION_TARGET} "ble32.dll" ${OPIFEX_OS_WINDOWS})

endmacro(add_opifex_libraries)


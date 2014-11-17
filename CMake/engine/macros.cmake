
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
	elseif("${OPIFEX_OS}" STREQUAL "OPIFEX_ANDROID")
		SET(BINARY_TARGET_DIRECTORY "android")
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

	if(${OPIFEX_RELEASE})
		SET(BINARY_RELEASE_MODE "release")
	else()
		SET(BINARY_RELEASE_MODE "debug")
	endif()
	
	if( ${OPIFEX_MATCH} )
		populate_binary_directory()
		add_custom_command(TARGET ${APPLICATION_TARGET} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${OPIFEX_BINARIES}/${BINARY_TARGET_DIRECTORY}/${BINARY_RELEASE_MODE}/${FILE_PATH}"
			${PROJECT_BINARY_DIR}/${BINARY_RELEASE_MODE})
			
	endif()

endmacro(copy_from_binaries_on_build)

macro(add_external_opifex_includes)
	include_directories(
		${OPIFEX_ENGINE_REPOSITORY}
		${OPIFEX_ENGINE_REPOSITORY}/External/glfw-3.0.4/include/
		${OPIFEX_ENGINE_REPOSITORY}/External/glew-1.9.0/include/
		${OPIFEX_ENGINE_REPOSITORY}/External/OpenAL/
		${OPIFEX_ENGINE_REPOSITORY}/External/Ogg/include/
		${OPIFEX_ENGINE_REPOSITORY}/External/Vorbis/include/
		${OPIFEX_ENGINE_REPOSITORY}/External/glm-0.9.5/
	)
endmacro(add_external_opifex_includes)

macro(add_opifex_definitions APPLICATION_TARGET APPLICATION_DIR_DEPTH )

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


	if(${OPIFEX_RELEASE})
		add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
		target_link_libraries(${APPLICATION_TARGET}	libcpmtd0.lib)
	endif()

endmacro(add_opifex_definitions)

macro(add_opifex_libraries APPLICATION_TARGET )

	unset(LIBLODEPNG CACHE)
	unset(LIBCORE CACHE)
	unset(LIBDATA CACHE)
	unset(LIBMATH CACHE)
	unset(LIBPERFORMANCE CACHE)
	unset(LIBSCRIPTING CACHE)
	unset(LIBPIPELINE CACHE)
	unset(LIBHUMAN CACHE)
	unset(LIBGLEW_158 CACHE)
	unset(LIBGLFW CACHE)
	unset(LIBOGG CACHE)
	unset(LIBVORBIS CACHE)
	unset(LIBVORBISFILE CACHE)
	
	if( ${OPIFEX_OS_ANDROID} )
		find_binary(LIBLODEPNG "LodePNG")
		find_binary(LIBCORE "opifex-core")
		find_binary(LIBDATA "opifex-data")
		find_binary(LIBMATH "Math")
		find_binary(LIBPERFORMANCE "Performance")
		find_binary(LIBSCRIPTING "Scripting")
		find_binary(LIBPIPELINE "Pipeline")
		find_binary(LIBHUMAN "Human")
		find_binary(LIBGLEW_158 "GLEW_158")
		find_binary(LIBGLFW "glfw3")
	else()

		find_binary(LIBLODEPNG "LodePNG")
		find_binary(LIBCORE "Core")
		find_binary(LIBDATA "Data")
		find_binary(LIBMATH "Math")
		find_binary(LIBPERFORMANCE "Performance")
		find_binary(LIBSCRIPTING "Scripting")
		find_binary(LIBPIPELINE "Pipeline")
		find_binary(LIBHUMAN "Human")
		find_binary(LIBGLEW_158 "GLEW_158")
		find_binary(LIBGLFW "glfw3")
	endif()

	target_link_libraries(${APPLICATION_TARGET}
		${OPENGL_LIBRARY}
		${LIBLODEPNG}
		${LIBCORE}
		${LIBDATA}
		${LIBMATH}
		${LIBPERFORMANCE}
		${LIBHUMAN}
		${LIBSCRIPTING}
		${LIBPIPELINE}
		${LIBGLFW}
		${LIBGLEW_158}
		${OPENAL_LIBRARY}
		${OpenGL}
	)

	if( ${OPIFEX_OS_WINDOWS} )
		if(${OPIFEX_RELEASE})
			find_binary(LIBOGG "libogg_static")
			find_binary(LIBVORBIS "libvorbis_static")
			find_binary(LIBVORBISFILE "libvorbisfile_static")
		else()
			find_binary(LIBOGG "libogg")
			find_binary(LIBVORBIS "libvorbis")
			find_binary(LIBVORBISFILE "libvorbisfile")
		endif()
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


		set(GL_LIBRARY GL GLU X11 Xi Xxf86vm Xrandr)

		target_link_libraries(${APPLICATION_TARGET}
			${X11_LIBRARIES}
			${CMAKE_THREAD_LIBS_INIT}
			${CMAKE_DL_LIBS}
			${OGG_LIBRARY}
			${VORBIS_LIBRARY}
			${VORBISFILE_LIBRARY}
			-logg
			${GL_LIBRARY}
			${GLFW_LIBRARIES}
		)
		SET(CMAKE_EXE_LINKER_FLAGS, "-ldl")
	endif()

	if(${OPIFEX_OS_OSX32} OR ${OPIFEX_OS_OSX64})
		SET(GLFW_LIBRARIES "/System/Library/Frameworks/Cocoa.framework;/System/Library/Frameworks/OpenGL.framework;/System/Library/Frameworks/IOKit.framework;/System/Library/Frameworks/CoreFoundation.framework;/System/Library/Frameworks/CoreVideo.framework")
		SET(OPENAL_LIBRARY "/System/Library/Frameworks/OpenAL.framework")
		target_link_libraries(${APPLICATION_TARGET}
			${CMAKE_THREAD_LIBS_INIT}
			${OPENAL_LIBRARY}
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

function(find_binary OPIFEX_LIBRARY OPIFEX_NAME)
	populate_binary_directory()

	if(${OPIFEX_OS_ANDROID})

		SET(OPIFEX_LIBRARY_NAME "lib${OPIFEX_NAME}.a")
		SET(${OPIFEX_LIBRARY} "${OPIFEX_BINARIES}/${BINARY_TARGET_DIRECTORY}/${OPIFEX_LIBRARY_NAME}")

		message(STATUS "SETTING '${OPIFEX_LIBRARY}' to '${OPIFEX_BINARIES}/${BINARY_TARGET_DIRECTORY}/${OPIFEX_LIBRARY_NAME}'")

		message(STATUS "	RESULT '${${OPIFEX_LIBRARY}}'")

	else()
		SET(OPIFEX_LIBRARY_NAME "lib${OPIFEX_NAME}.a")

		if(${OPIFEX_OS_WINDOWS})
			SET(OPIFEX_LIBRARY_NAME "${OPIFEX_NAME}.lib")
		endif()

		if(${OPIFEX_OS_ANDROID})
			SET(BINARY_RELEASE_MODE "")
		else()
			if(${OPIFEX_RELEASE})
				SET(BINARY_RELEASE_MODE "release/")
			else()
				SET(BINARY_RELEASE_MODE "debug/")
			endif()
		endif()


		find_library( ${OPIFEX_LIBRARY} NAMES ${OPIFEX_LIBRARY_NAME} PATHS 
		"${OPIFEX_BINARIES}/${BINARY_TARGET_DIRECTORY}/${BINARY_RELEASE_MODE}" PARENT_SCOPE)

		message(STATUS "Looking for '${OPIFEX_LIBRARY_NAME}' in '${OPIFEX_BINARIES}/${BINARY_TARGET_DIRECTORY}/${BINARY_RELEASE_MODE}'")
		message(STATUS "		Found: '${${OPIFEX_LIBRARY}}'")

	endif()
endfunction(find_binary)


macro(opifex_engine_status_messages)

	message(STATUS "")
	message(STATUS "===================================")
	message(STATUS "==    OPifex CMake Completed     ==")
	message(STATUS "===================================\n\nCONFIGURED VARIABLES:\n")
	message(STATUS "  OPIFEX_REPOSITORY: ${OPIFEX_REPOSITORY}")
	message(STATUS "  OPIFEX_ENGINE_REPOSITORY: ${OPIFEX_ENGINE_REPOSITORY}")
	message(STATUS "  OPIFEX_BINARIES: ${OPIFEX_BINARIES}")
	message(STATUS "  OPIFEX_REPO: ${OPIFEX_REPO}")
	message(STATUS "  OPIFEX_OS: ${OPIFEX_OS}")
	message(STATUS "  PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")
	message(STATUS "  PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
	message(STATUS "")
	message(STATUS "===================================\n\nCONFIGURED:\n")
	message(STATUS "  OPENAL_LIBRARY: ${OPENAL_LIBRARY}")
	message(STATUS "  GLFW_LIBRARIES: ${GLFW_LIBRARIES}")
	message(STATUS "  OGG_LIBRARY: ${OGG_LIBRARY}")
	message(STATUS "  VORBISFILE_LIBRARY: ${VORBISFILE_LIBRARY}")
	message(STATUS "  GLFW_LIBRARIES: ${GLFW_LIBRARIES}")
	message(STATUS "")
	message(STATUS "===================================\n\nCONFIGURED BINARIES:\n")
	message(STATUS "  LIBLODEPNG: ${LIBLODEPNG}")
	message(STATUS "  LIBCORE: ${LIBCORE}")
	message(STATUS "  LIBDATA: ${LIBDATA}")
	message(STATUS "  LIBMATH: ${LIBMATH}")
	message(STATUS "  LIBPERFORMANCE: ${LIBPERFORMANCE}")
	message(STATUS "  LIBSCRIPTING: ${LIBSCRIPTING}")
	message(STATUS "  LIBPIPELINE: ${LIBPIPELINE}")
	message(STATUS "  LIBHUMAN: ${LIBHUMAN}")
	message(STATUS "  LIBGLEW_158: ${LIBGLEW_158}")
	message(STATUS "  LIBGLFW: ${LIBGLFW}")
	message(STATUS "  LIBOGG: ${LIBOGG}")
	message(STATUS "  LIBVORBIS: ${LIBVORBIS}")
	message(STATUS "  LIBVORBISFILE: ${LIBVORBISFILE}")
	message(STATUS "")
	message(STATUS "===================================\n")

endmacro(opifex_engine_status_messages)


if(${OPIFEX_RELEASE})
	SET(BINARY_RELEASE_MODE "release")
else()
	SET(BINARY_RELEASE_MODE "debug")
endif()

macro(copy_file_to_binaries FILE_TO_COPY)

		file(COPY 
			${OPIFEX_ENGINE_REPOSITORY}${FILE_TO_COPY}
			DESTINATION ${PROJECT_BINARY_DIR}/Binaries/${BINARY_TARGET_DIRECTORY}/${BINARY_RELEASE_MODE}/)

endmacro(copy_file_to_binaries)
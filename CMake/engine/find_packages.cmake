
### Find common packages depending on the System


set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")

if(${OPIFEX_OS_IOS})

else()

	include(FindOpenGL)
	find_package(OpenGL REQUIRED)


	if(${OPIFEX_OPTION_AUDIO})
		include(${CMAKE_CURRENT_LIST_DIR}/find/OpenAL.cmake)
		include(${CMAKE_CURRENT_LIST_DIR}/find/OggVorbis.cmake)
	endif()

	if(APPLE)
		include(${CMAKE_CURRENT_LIST_DIR}/find/IOKit.cmake)
		find_package(Threads)
	endif()

endif()

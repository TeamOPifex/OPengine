
### Find common packages depending on the System


set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")

if(${OPIFEX_OS_IOS})

else()

	include(FindOpenGL)
	find_package(OpenGL REQUIRED)


	include(${CMAKE_CURRENT_LIST_DIR}/find/OpenAL.cmake)

	if(APPLE)
		include(${CMAKE_CURRENT_LIST_DIR}/find/OggVorbis.cmake)
		include(${CMAKE_CURRENT_LIST_DIR}/find/IOKit.cmake)
		find_package(Threads)
	elseif(UNIX)
		include(${CMAKE_CURRENT_LIST_DIR}/find/OggVorbis.cmake)
	else()
		include(${CMAKE_CURRENT_LIST_DIR}/find/OggVorbis.cmake)
	endif()

endif()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")

if(${OPIFEX_OS_IOS})

else()
	include(FindOpenGL)
	find_package(OpenGL REQUIRED)

	find_package(OpenAL REQUIRED)
	if(APPLE)
		find_package(OggVorbis REQUIRED)
		find_package(IOKit REQUIRED)
		find_package(Threads)
	elseif(UNIX)
		find_package(OggVorbis REQUIRED)
	else()
		find_package(OggVorbis REQUIRED)
	endif()

endif()

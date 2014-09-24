include(FindOpenGL)
find_package(OpenGL REQUIRED)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")
find_package(OpenAL REQUIRED)
if(APPLE)
	find_package(IOKit REQUIRED)
	find_package(Threads)
	find_package(OggVorbis REQUIRED)
elseif(UNIX)
	find_package(OggVorbis REQUIRED)
else()
	find_package(OggVorbis REQUIRED)
endif()

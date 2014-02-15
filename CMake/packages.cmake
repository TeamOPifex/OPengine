include(FindOpenGL)
find_package(OpenGL REQUIRED)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")
find_package(OpenAL REQUIRED)
find_package(OggVorbis REQUIRED)
find_package(IOKit REQUIRED)
find_package(Threads)
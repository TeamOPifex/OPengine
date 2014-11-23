set(OPIFEX_ENGINE_REPOSITORY "../OPifex.Engine" CACHE STRING "Directory to Engine Repository from Output Folder")
set(OPIFEX_BINARIES "../Engine/Binaries" CACHE STRING "Directory to Binaries from Output Folder")

include(CMake/engine/opifex.cmake)
include(CMake/assets.cmake)
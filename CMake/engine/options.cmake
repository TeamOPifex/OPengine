
if(OPIFEX_BINARIES)

else()
    option(OPIFEX_OPTION_OCULUS "Build the Oculus library into the engine" OFF)
    option(OPIFEX_OPTION_MYO "Build the Myo library into the engine" OFF)
    option(OPIFEX_OPTION_V8 "Build the V8 library into the engine" OFF)
    option(OPIFEX_OPTION_PHYSX "Build the PhysX library into the engine" OFF)
    option(OPIFEX_OPTION_SPINE "Build the Spine library into the engine" OFF)
    option(OPIFEX_OPTION_FMOD "Build the FMod library into the engine" OFF)
    option(OPIFEX_OPTION_AUDIO "Build the Audio into the engine" OFF)
endif()

option(OPIFEX_OPTION_NODEJS "Configure build files for JavaScript-0_12-0_12.js module" OFF)
option(OPIFEX_OPTION_RELEASE "Build in release mode" OFF)
option(OPIFEX_OPTION_SHARED "Build shared libraries" OFF)
{
    "targets": [
        {
            "target_name": "OPengine",
            "sources": [
                 "../OPengine/Scripting/src/JavaScript/NodeWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/OPjavaScriptCoreWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Core/OPdefinedWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Core/OPtimerWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/OPjavaScriptDataWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Data/OPcmanWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/OPjavaScriptMathWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Math/OPmat4Wrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Math/OPvec2Wrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Math/OPvec3Wrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Math/OPvec4Wrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/OPjavaScriptPerformanceWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Performance/OPphysXWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Performance/OPphysXControllerWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Performance/OPphysXSceneWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/OPjavaScriptHumanWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Human/OPATTRWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Human/OPcamFreeFlightWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Human/OPcamWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Human/OPcubeWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Human/OPeffectWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Human/OPfontWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Human/OPfontManagerWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Human/OPfontRenderWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Human/OPgamePadWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Human/OPkeyboardWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Human/OPmaterialWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Human/OPmeshWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Human/OPmodelWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Human/OPrenderWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Human/OPskeletonWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Human/OPskeletonAnimationWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Human/OPtextureWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/OPjavaScriptCommunicationWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/OPjavaScriptPipelineWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Pipeline/OPloaderWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Pipeline/OPloaderVoxelsWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Pipeline/OPvoxelGeneratorWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Pipeline/OPtexture2DWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Pipeline/OPsprite2DWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Pipeline/OPspriteSystemWrapper.cpp",
                 "../OPengine/Scripting/src/JavaScript/Pipeline/OPfmodWrapper.cpp"
            ],
            "include_dirs": [
                "@PROJECT_SOURCE_DIR@/",
                "@BINARY_FOLDER@/",
                "@PROJECT_SOURCE_DIR@/External/glew-1.9.0/include",
                "@PROJECT_SOURCE_DIR@/External/glfw/include",
                "@PROJECT_SOURCE_DIR@/External/PhysX/include",
                "@PROJECT_SOURCE_DIR@/External/FMod/include",
                "@PROJECT_SOURCE_DIR@/External/OpenAL/",
                "@PROJECT_SOURCE_DIR@/External/Myo/include/"
            ],
            "cflags!": [
                "-fno-exceptions"
            ],
            "cflags_cc!": [
                "-fno-exceptions"
            ],
            "conditions": [
                [
                    "OS=='win'",
                    {
                        'defines': [
                          'OPIFEX_OPTION_NODEJS',
                          "_ITERATOR_DEBUG_LEVEL=0",
                          "OPIFEX_NODEJS_@OPIFEX_NODE_VERSION@"
                        ],
                        "link_settings": {
                            "libraries": [
                                "-lCore.lib",
                                "-lData.lib",
                                "-lMath.lib",
                                "-lPerformance.lib",
                                "-lHuman.lib",
                                "-lCommunication.lib",
                                "-lPipeline.lib",
                                "-lglfw3.lib",
                                "-lGLEW_158.lib",
                                "-lLodePNG.lib",
                                "-lopengl32.lib",
                                "-lWinmm.lib",
                                "-lOpenAL32.lib",
                                "-lfmod.lib"
                            ],
                            "library_dirs": [
                                "@BINARY_FOLDER@/"
                            ]
                        },
                        "configurations": {
                            "Release": {
                                "msvs_settings": {
                                    "VCCLCompilerTool": {
                                        "ExceptionHandling": "0",
                                        "AdditionalOptions": [
                                            "/MP /EHsc"
                                        ]
                                    },
                                    "VCLibrarianTool": {
                                        "AdditionalOptions": [
                                            "/LTCG /NODEFAULTLIB:MSVCRT"
                                        ]
                                    },
                                    "VCLinkerTool": {
                                        "IgnoreDefaultLibraryNames": [ 'libcmt.lib', 'libcpmt.lib' ],
                                        "LinkTimeCodeGeneration": 1,
                                        "LinkIncremental": 1,
                                        "AdditionalLibraryDirectories": [
                                            "../Core/Release/",
                                            "../Data/Release/",
                                            "../Math/Release/",
                                            "../Performance/Release/",
                                            "../Human/Release/",
                                            "../Communication/Release/",
                                            "../Pipeline/Release/",
                                            "../GLFW/src/Release/",
                                            "../External/Release/",
                                            "../Binaries/win64/release/",
                                            "../@PROJECT_SOURCE_DIR@/External/Ogg/lib/win32/",
                                            "../@PROJECT_SOURCE_DIR@/External/Vorbis/lib/win32/",
                                            "C:/Program Files (x86)/OpenAL 1.1 SDK/libs/Win32/",
                                        ]
                                    }
                                }
                            }
                        }
                    },
                ],
                [
                    "OS=='mac'",
                    {
                        'defines': [
                          'OPIFEX_OPTION_NODEJS',
                          "_ITERATOR_DEBUG_LEVEL=0",
                          "OPIFEX_NODEJS_@OPIFEX_NODE_VERSION@"
                        ],
                        "link_settings": {
                            "libraries": [
                                "libCore.a",
                                "libData.a",
                                "libMath.a",
                                "libPerformance.a",
                                "libHuman.a",
                                "libCommunication.a",
                                "libPipeline.a",
                                "libglfw3.a",
                                "libGLEW_158.a",
                                "libLodePNG.a",
                                "libLowLevelDEBUG.a",
                                "libLowLevelClothDEBUG.a",
                                "libPhysX3CharacterKinematicDEBUG.a",
                                "libPhysX3CommonDEBUG.a",
                                "libPhysX3CookingDEBUG.a",
                                "libPhysX3DEBUG.a",
                                "libPhysX3ExtensionsDEBUG.a",
                                "libPhysX3VehicleDEBUG.a",
                                "libPhysXProfileSDKDEBUG.a",
                                "libPhysXVisualDebuggerSDKDEBUG.a",
                                "libPvdRuntimeDEBUG.a",
                                "libPxTaskDEBUG.a",
                                "libSceneQueryDEBUG.a",
                                "libSimulationControllerDEBUG.a",
                                "/System/Library/Frameworks/Cocoa.framework",
                                "/System/Library/Frameworks/OpenGL.framework",
                                "/System/Library/Frameworks/IOKit.framework",
                                "/System/Library/Frameworks/CoreFoundation.framework",
                                "/System/Library/Frameworks/CoreVideo.framework",
                                "/System/Library/Frameworks/OpenAL.framework",
                                "/System/Library/Frameworks/myo.framework",
                                "/usr/local/lib/libogg.dylib",
                                "/usr/local/lib/libvorbisfile.dylib",
                                "/usr/local/lib/libfmod.dylib"
                            ],
                            "library_dirs": [
                                "../Binaries/osx64/debug/"
                            ],
                        }
                    }
                ]
            ]
        },
        {
            'target_name': 'action_after_build',
            'type': 'none',
            'copies': [
                {
                    'files': [ '<(PRODUCT_DIR)/OPengine.node' ],
                    'destination': '@BINARY_FOLDER@'
                }
            ],
        }
    ]
}

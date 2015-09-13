{
    "targets": [
        {
            "target_name": "OPengine",
            "sources": [
                '<!@(ls -1 ../OPengine/Scripting/src/JavaScript/*.cpp)',
                '<!@(ls -1 ../OPengine/Scripting/src/JavaScript/**/*.cpp)'
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
            "cflags": [
                "@CMAKE_CXX_FLAGS@"
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
                                "-lScripting.lib",
                                "-lglfw3.lib",
                                "-lGLEW_158.lib",
                                "-lLodePNG.lib",
                                "-lopengl32.lib",
                                "-lWinmm.lib",
                                "-lOpenAL32.lib",
                                "-lfmod64_vc.lib",
                                "-lLowLevelDEBUG.lib",
                                "-lLowLevelClothDEBUG.lib",
                                "-lPhysX3CharacterKinematic_x64.lib",
                                "-lPhysX3Common_x64.lib",
                                "-lPhysX3Cooking_x64.lib",
                                "-lPhysX3_x64.lib",
                                "-lPhysX3Extensions.lib",
                                "-lPhysX3Vehicle.lib",
                                "-lPhysXProfileSDK.lib",
                                "-lPhysXVisualDebuggerSDK.lib",
                                "-lPvdRuntime.lib",
                                "-lPxTask.lib",
                                "-lSceneQuery.lib",
                                "-lSimulationController.lib",
                                "-lXInput9_1_0.lib"
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

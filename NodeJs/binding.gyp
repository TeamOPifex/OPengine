{
    "targets": [
        {
            "target_name": "OPengine",
            "sources": [
                "Wrapper.cpp",
                 "../OPengine/Scripting/src/Node/OPscriptNodeWrapperCore.cpp",
                 "../OPengine/Scripting/src/Node/OPscriptNodeWrapperData.cpp",
                 "../OPengine/Scripting/src/Node/OPscriptNodeWrapperMath.cpp",
                 "../OPengine/Scripting/src/Node/Math/OPmat4Wrapper.cpp",
                 "../OPengine/Scripting/src/Node/Math/OPvec3Wrapper.cpp",
                 "../OPengine/Scripting/src/Node/OPscriptNodeWrapperPerformance.cpp",
                 "../OPengine/Scripting/src/Node/OPscriptNodeWrapperHuman.cpp",
                 "../OPengine/Scripting/src/Node/Human/OPcamWrapper.cpp",
                 "../OPengine/Scripting/src/Node/OPscriptNodeWrapperCommunication.cpp",
                 "../OPengine/Scripting/src/Node/OPscriptNodeWrapperPipeline.cpp"
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
                          "_ITERATOR_DEBUG_LEVEL=0"
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
                                "-llibogg.lib",
                                "-llibvorbis.lib",
                                "-llibvorbisfile.lib",
                                "-lOpenAL32.lib"
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
                                "/System/Library/Frameworks/Cocoa.framework",
                                "/System/Library/Frameworks/OpenGL.framework",
                                "/System/Library/Frameworks/IOKit.framework",
                                "/System/Library/Frameworks/CoreFoundation.framework",
                                "/System/Library/Frameworks/CoreVideo.framework",
                                "/System/Library/Frameworks/OpenAL.framework",
                                "/System/Library/Frameworks/myo.framework",
                                "/usr/local/lib/libogg.dylib",
                                "/usr/local/lib/libvorbisfile.dylib"
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
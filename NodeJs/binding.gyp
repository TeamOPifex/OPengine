{
    "targets": [
        {
            "target_name": "OPengine",
            "sources": [
                "Wrapper.cpp",
                "@PROJECT_SOURCE_DIR@/Scripting/src/wrappers/Global.cpp",
                "@PROJECT_SOURCE_DIR@/Scripting/src/wrappers/DataWrapper.cpp",
                "@PROJECT_SOURCE_DIR@/Scripting/src/wrappers/MathWrapper.cpp",
				"@PROJECT_SOURCE_DIR@/Scripting/src/wrappers/HumanWrapper.cpp",
				"@PROJECT_SOURCE_DIR@/Scripting/src/wrappers/V8Helper.cpp"
            ],
            "include_dirs": [
                "@PROJECT_SOURCE_DIR@/",
                "@BINARY_FOLDER@/",
                "@PROJECT_SOURCE_DIR@/External/glew-1.9.0/include",
                "@PROJECT_SOURCE_DIR@/External/glfw-3.0.4/include",
                "@PROJECT_SOURCE_DIR@/External/glm-0.9.1",
                "@PROJECT_SOURCE_DIR@/External/V8/",
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
                          'OPIFEX_WIN32',
                          'OPIFEX_V8',
                          'OPIFEX_NODEJS',
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
                          'OPIFEX_OSX64',
                          'OPIFEX_V8',
                          'OPIFEX_NODEJS',
                          'OPIFEX_MYO',
                          "_ITERATOR_DEBUG_LEVEL=0"
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
        }
    ]
}
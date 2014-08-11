{
    "targets": [
        {
            "target_name": "OPengine",
            "sources": [
                "Wrapper.cpp",
                "@OPIFEX_REPOSITORY@/Scripting/src/wrappers/Global.cpp",
                "@OPIFEX_REPOSITORY@/Scripting/src/wrappers/DataWrapper.cpp",
                "@OPIFEX_REPOSITORY@/Scripting/src/wrappers/MathWrapper.cpp",
				"@OPIFEX_REPOSITORY@/Scripting/src/wrappers/HumanWrapper.cpp",
				"@OPIFEX_REPOSITORY@/Scripting/src/wrappers/V8Helper.cpp"
            ],
            "include_dirs": [
                "@OPIFEX_REPOSITORY@/",
                "@OPIFEX_REPOSITORY@/External/glew-1.9.0/include",
                "@OPIFEX_REPOSITORY@/External/glfw-3.0.4/include",
                "@OPIFEX_REPOSITORY@/External/glm-0.9.1"
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
                                            "Core/Release/",
                                            "Data/Release/",
                                            "Math/Release/",
                                            "Performance/Release/",
                                            "Human/Release/",
                                            "Communication/Release/",
                                            "Pipeline/Release/",
                                            "GLFW/src/Release/",
                                            "External/Release/",
                                            "@OPIFEX_REPOSITORY@/External/Ogg/lib/win32/",
                                            "@OPIFEX_REPOSITORY@/External/Vorbis/lib/win32/",
                                            "C:/Program Files (x86)/OpenAL 1.1 SDK/libs/Win32/",
                                        ]
                                    }
                                }
                            }
                        }
                    }
                ]
            ]
        }
    ]
}
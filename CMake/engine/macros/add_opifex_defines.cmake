
macro(add_opifex_defines)

	if(${OPIFEX_OPTION_MYO})
		add_definitions(-DOPIFEX_OPTION_MYO)
	endif()

	if(${OPIFEX_OPTION_OCULUS})
		add_definitions(-DOPIFEX_OPTION_OCULUS)
	endif()

	if(${OPIFEX_OPTION_PHYSX})
		add_definitions(-DOPIFEX_OPTION_PHYSX)
	endif()

	if(${OPIFEX_OPTION_RELEASE})
		add_definitions(-DOPIFEX_OPTION_RELEASE)
	endif()

	if(${OPIFEX_OPTION_SHARED})
		add_definitions(-DOPIFEX_OPTION_SHARED)
	endif()

	if(${OPIFEX_OPTION_SPINE})
		add_definitions(-DOPIFEX_OPTION_SPINE)
	endif()

	if(${OPIFEX_OPTION_FMOD})
		add_definitions(-DOPIFEX_OPTION_FMOD)
	endif()

	if(${OPIFEX_OPTION_AUDIO})
		add_definitions(-DOPIFEX_OPTION_AUDIO)
	endif()

	if(${OPIFEX_OPTION_V8})
		add_definitions(-DOPIFEX_OPTION_V8)
	endif()

	add_definitions(-DOPIFEX_${OPENGL_DESKTOP_TARGET})
	add_definitions(-D${OPIFEX_OS})

	if(${OPIFEX_OPTION_RELEASE})
	else()
		add_definitions(-D_DEBUG)
	endif()

	if(${OPIFEX_OPTION_NODEJS})
		add_definitions(-DOPIFEX_JAVASCRIPT)
	endif()

	if(${OPIFEX_OPTION_EMSCRIPTEN})
		add_definitions(-DOPIFEX_EMSCRIPTEN)
	endif()

	if(${OPIFEX_OPTION_V8})
		add_definitions(-DOPIFEX_JAVASCRIPT)
	endif()

	add_opifex_assets()

	if(NOT ${OPIFEX_OS_IOS})
		add_definitions(-DGLEW_STATIC)
	endif()

endmacro(add_opifex_defines)

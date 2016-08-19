#
# macro(add_opifex_assimp_set_source)
#
# 	# Look for an External Path to the Assimp folder
# 	SET(_ASSIMP_SOURCE 0)
# 	if(NOT "${ASSIMP_PATH}" STREQUAL "")
# 		# message(STATUS "Assimp PATH WAS SET ${ASSIMP_PATH}")
# 		SET(_ASSIMP_SOURCE 1)
# 		SET(_ASSIMP_PATH "${ASSIMP_PATH}")
# 	else()
# 		# message(STATUS "Assimp PATH WAS NOT SET ${ASSIMP_PATH}")
# 		SET(_ASSIMP_PATH "${PROJECT_SOURCE_DIR}/External/Assimp")
# 	endif()
#
# endmacro(add_opifex_assimp_set_source)
#
#
# macro(add_opifex_assimp_include)
#
# 	add_opifex_assimp_set_source()
# 	include_directories(${_ASSIMP_PATH}/include/)
# 	# message(STATUS "${_ASSIMP_PATH}/include/")
#
# endmacro(add_opifex_assimp_include)
#
#
# function(add_opifex_assimp APPLICATION_TARGET )
#
# 	add_opifex_assimp_set_source()
#
# 	# message(STATUS "Link Assimp")
#
#   if(${OPIFEX_OS_WINDOWS})
# 		add_opifex_assimp_windows(${APPLICATION_TARGET})
#   else()
# 		add_opifex_assimp_osx(${APPLICATION_TARGET})
# 	endif()
#
# endfunction(add_opifex_assimp)
#
#
# macro(add_opifex_assimp_osx APPLICATION_TARGET)
#
#
#
# 	if(${OPIFEX_OS_OSX64})
#
# 		SET(_ASSIMP_BINARY_LOCATION "${OPIFEX_ENGINE_REPOSITORY}/External/Assimp/lib/debug/osx64/")
# 		if(_ASSIMP_SOURCE)
# 			SET(_ASSIMP_BINARY_LOCATION "${ASSIMP_PATH}/Lib/osx64/")
# 		endif()
#
# 		# message("${ASSIMP_PATH} ${_ASSIMP_SOURCE}")
#
# 	elseif(${OPIFEX_OS_OSX32})
#
#
# 	endif()
#
# endmacro(add_opifex_assimp_osx)
#
#
# macro(add_opifex_assimp_windows APPLICATION_TARGET)
#
# 	if(${OPIFEX_OS_WIN64})
#
# 		SET(_ASSIMP_BINARY_LOCATION "${OPIFEX_ENGINE_REPOSITORY}/External/Assimp/lib/debug/win64/")
#   		SET(_ASSIMP_DLL_LOCATION "${OPIFEX_ENGINE_REPOSITORY}/External/Assimp/lib/debug/win64/")
# 		if(_ASSIMP_SOURCE)
# 			SET(_ASSIMP_BINARY_LOCATION "${ASSIMP_PATH}/lib64/")
#   		SET(_ASSIMP_DLL_LOCATION "${ASSIMP_PATH}/bin64/")
# 		endif()
#
#     copy_to_binaries(${ASSIMP_PATH}/bin64/assimp.dll)
#     copy_to_binaries(${_ASSIMP_BINARY_LOCATION}assimp.lib)
#
#     link_from_binaries(${APPLICATION_TARGET}
#       assimp.lib)
#
# 	else()
#
# 		SET(_ASSIMP_BINARY_LOCATION "${OPIFEX_ENGINE_REPOSITORY}/External/Assimp/lib/debug/win32/")
#   	SET(_ASSIMP_DLL_LOCATION "${OPIFEX_ENGINE_REPOSITORY}/External/Assimp/lib/debug/win32/")
# 		if(_ASSIMP_SOURCE)
# 			SET(_ASSIMP_BINARY_LOCATION "${ASSIMP_PATH}/lib32/")
#   		SET(_ASSIMP_DLL_LOCATION "${ASSIMP_PATH}/bin32/")
# 		endif()
#
#     copy_to_binaries(${_ASSIMP_DLL_LOCATION}/assimp.dll)
#     copy_to_binaries(${_ASSIMP_BINARY_LOCATION}assimp.lib)
#
#     link_from_binaries(${APPLICATION_TARGET} assimp.lib)
#
# 	endif()
#
# endmacro(add_opifex_assimp_windows)

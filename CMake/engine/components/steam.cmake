macro(add_opifex_steam APPLICATION_TARGET )

	if(${OPIFEX_OPTION_STEAM})

		message(STATUS "Link Steam")
		include_directories(../External/Steam/include/)

		message(STATUS "Steam")
		target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/Steam/lib/win32/steam_api.lib)

		copy_to_folder(${APPLICATION_TARGET} "/External/Steam/lib/" "steam_api.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})

	endif()

endmacro(add_opifex_steam)

macro(add_opifex_assets APPLICATION_TARGET )

	unset(OPIFEX_ASSETS)
	
	if(${OPIFEX_OPTION_RELEASE})
		SET(OPIFEX_ASSETS NULL)
		add_definitions(-DOPIFEX_ASSETS=NULL)
	else()

		add_definitions(-D_DEBUG)
		SET(OPIFEX_ASSETS "${PROJECT_SOURCE_DIR}/Assets/")
		add_definitions(-DOPIFEX_ASSETS="${OPIFEX_ASSETS}")

		message(STATUS "OPIFEX ASSETS SET TO: ${OPIFEX_ASSETS}")

	endif()
	
endmacro(add_opifex_assets)

macro(add_opifex_physics APPLICATION_TARGET )

	if(${OPIFEX_OPTION_PHYSICS}) 

		message(STATUS "Link Physics")
		include_directories(../External/PhysX/include/)

		if(${OPIFEX_OPTION_RELEASE})

			message(STATUS "Physics RELEASE")
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/PhysX3_x86.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/PhysX3Common_x86.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/PhysX3Extensions.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/PxTask.lib)

			copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/release/" "PhysX3_x86.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
			copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/release/" "PhysX3Common_x86.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})

		else()

			message(STATUS "Physics DEBUG")
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/PhysX3DEBUG_x86.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/PhysX3CommonDEBUG_x86.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/PhysX3ExtensionsDEBUG.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/PxTaskDEBUG.lib)

			copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/debug/" "PhysX3CHECKED_x86.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
			copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/debug/" "PhysX3CommonCHECKED_x86.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
			copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/debug/" "nvToolsExt32_1.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})

		endif()
	endif()

endmacro(add_opifex_physics)


macro(add_opifex_v8 APPLICATION_TARGET )

	if(${OPIFEX_OPTION_V8})

		if(${OPIFEX_OS_WINDOWS})
			target_link_libraries(${APPLICATION_TARGET} 
				ws2_32.lib
				advapi32.lib
				winmm.lib)
			if(${OPIFEX_OPTION_RELEASE})
				if(${OPIFEX_OS_64})
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/release/win64/v8.lib)
				else()
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/release/win32/v8.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/release/win32/v8_libbase.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/release/win32/v8_libplatform.lib)
				endif()

			else()
			
				if(${OPIFEX_OS_64})
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win64/v8.lib)
				else()
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win32/v8.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win32/v8_libbase.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win32/v8_libplatform.lib)
				endif()
			
			endif()


			if(${OPIFEX_OPTION_RELEASE})
				copy_to_folder(${APPLICATION_TARGET} "/External/V8/lib/release/" "icui18n.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
				copy_to_folder(${APPLICATION_TARGET} "/External/V8/lib/release/" "icuuc.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
				copy_to_folder(${APPLICATION_TARGET} "/External/V8/lib/release/" "v8.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
			else()	
				copy_to_folder(${APPLICATION_TARGET} "/External/V8/lib/debug/" "icui18n.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
				copy_to_folder(${APPLICATION_TARGET} "/External/V8/lib/debug/" "icuuc.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
				copy_to_folder(${APPLICATION_TARGET} "/External/V8/lib/debug/" "v8.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
			endif()
		else()

			SET(CMAKE_C_FLAGS "-stdlib=libstdc++"	)
			SET(CMAKE_CXX_FLAGS "-stdlib=libstdc++"	)

			if(${OPIFEX_OPTION_RELEASE})

				if(${OPIFEX_OS_64})
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/release/osx64/v8.lib)
				else()
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/release/win32/v8.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/release/win32/v8_libbase.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/release/win32/v8_libplatform.lib)
				endif()

			else()

				copy_file_to_binaries(/External/V8/lib/debug/osx64/libv8_base.a)
				copy_file_to_binaries(/External/V8/lib/debug/osx64/libv8_libbase.a)
				copy_file_to_binaries(/External/V8/lib/debug/osx64/libv8_libplatform.a)
				copy_file_to_binaries(/External/V8/lib/debug/osx64/libv8_nosnapshot.a)
				copy_file_to_binaries(/External/V8/lib/debug/osx64/libicudata.a)
				copy_file_to_binaries(/External/V8/lib/debug/osx64/libicuuc.a)
				copy_file_to_binaries(/External/V8/lib/debug/osx64/libicui18n.a)
				
				if(${OPIFEX_OS_64})
					target_link_libraries(${APPLICATION_TARGET} 
						${PROJECT_SOURCE_DIR}/External/V8/lib/debug/osx64/libv8_nosnapshot.a)

					target_link_libraries(${APPLICATION_TARGET} 
						${PROJECT_SOURCE_DIR}/External/V8/lib/debug/osx64/libv8_base.a)

					target_link_libraries(${APPLICATION_TARGET} 
						${PROJECT_SOURCE_DIR}/External/V8/lib/debug/osx64/libv8_libbase.a)

					target_link_libraries(${APPLICATION_TARGET} 
						${PROJECT_SOURCE_DIR}/External/V8/lib/debug/osx64/libv8_libplatform.a)

					target_link_libraries(${APPLICATION_TARGET} 
						${PROJECT_SOURCE_DIR}/External/V8/lib/debug/osx64/libicudata.a)
					
					target_link_libraries(${APPLICATION_TARGET} 
						${PROJECT_SOURCE_DIR}/External/V8/lib/debug/osx64/libicuuc.a)

					target_link_libraries(${APPLICATION_TARGET} 
						${PROJECT_SOURCE_DIR}/External/V8/lib/debug/osx64/libicui18n.a)

					message(STATUS "LIBV8")
				else()
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win32/v8.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win32/v8_libbase.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win32/v8_libplatform.lib)
				endif()
			
			endif()
		endif()

	endif()

endmacro(add_opifex_v8)


macro(add_opifex_spine APPLICATION_TARGET )

	if(${OPIFEX_OPTION_SPINE})
		target_link_libraries(${APPLICATION_TARGET} SPINE)
	endif()
	
endmacro(add_opifex_spine)


macro(add_opifex_oculus APPLICATION_TARGET )

	if(${OPIFEX_OPTION_OCULUS})
		include_directories(../External/Oculus/include/	../External/Oculus/include/)
		target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/OculusDK2/lib/win32/VS2010/libovrd.lib)
	endif()
	
endmacro(add_opifex_oculus)


macro(add_opifex_myo APPLICATION_TARGET )

	if(${OPIFEX_OPTION_MYO})
		include_directories(../External/Myo/include/)
		if(${OPIFEX_OS_WIN64})
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/Myo/lib/win64/myo64.lib)
			copy_to_folder(${APPLICATION_TARGET}  "/External/Myo/lib/" "myo64.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
			copy_to_folder(${APPLICATION_TARGET}  "/External/Myo/lib/" "ble64.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
		elseif(${OPIFEX_OS_WIN32})
			target_link_libraries(${APPLICATION_TARGET}  ${PROJECT_SOURCE_DIR}/External/Myo/lib/win32/myo32.lib)
			copy_to_folder(${APPLICATION_TARGET}  "/External/Myo/lib/" "myo32.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
			copy_to_folder(${APPLICATION_TARGET}  "/External/Myo/lib/" "ble32.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
		elseif(${OPIFEX_OS_OSX64})
			target_link_libraries(${APPLICATION_TARGET}  ${PROJECT_SOURCE_DIR}/External/Myo/lib/osx64/myo.framework)
		endif()
	endif()
	
endmacro(add_opifex_myo)



macro(add_opifex_oggvorbis APPLICATION_TARGET )

	if(${OPIFEX_OPTION_RELEASE})
		if(${OPIFEX_WINDOWS})
			add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
			target_link_libraries(${APPLICATION_TARGET}	libcpmtd0.lib)
			target_link_libraries(${APPLICATION_TARGET}	${OGG_LIBRARY_RELEASE} ${VORBIS_LIBRARY_RELEASE} ${VORBISFILE_LIBRARY_RELEASE})
		endif()
	else()
		target_link_libraries(${APPLICATION_TARGET}	${OGG_LIBRARY} ${VORBIS_LIBRARY} ${VORBISFILE_LIBRARY})
	endif()

	if(${OPIFEX_OPTION_RELEASE})
		copy_to_folder(${APPLICATION_TARGET} "/External/Ogg/lib/release/" "libogg.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
		copy_to_folder(${APPLICATION_TARGET} "/External/Vorbis/lib/release/" "libvorbis.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
		copy_to_folder(${APPLICATION_TARGET} "/External/Vorbis/lib/release/" "libvorbisfile.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
	else()
		copy_to_folder_msvc(${APPLICATION_TARGET} "/External/Ogg/lib/debug/" "libogg.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
		copy_to_folder_msvc(${APPLICATION_TARGET} "/External/Vorbis/lib/debug/" "libvorbis.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
		copy_to_folder_msvc(${APPLICATION_TARGET} "/External/Vorbis/lib/debug/" "libvorbisfile.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
	endif()

		
	if(${OPIFEX_OS_WIN64})
		if(${OPIFEX_OPTION_RELEASE})
			message(STATUS "Copying... OggVorbis Win64 Release")
			copy_file_to_binaries(/External/Ogg/lib/release/win64/ogg.dll)
			copy_file_to_binaries(/External/Ogg/lib/release/win64/libogg.dll)
			copy_file_to_binaries(/External/Vorbis/lib/release/win64/vorbisfile.dll)
			copy_file_to_binaries(/External/Vorbis/lib/release/win64/libvorbis.dll)
			copy_file_to_binaries(/External/Vorbis/lib/release/win64/libvorbisfile.dll)

			copy_file_to_binaries(/External/Ogg/lib/release/win64/libogg_static.lib)
			copy_file_to_binaries(/External/Vorbis/lib/release/win64/libvorbis_static.lib)
			copy_file_to_binaries(/External/Vorbis/lib/release/win64/libvorbisfile_static.lib)

		else()

			message(STATUS "Copying... OggVorbis Win64 Debug")
			copy_file_to_binaries(/External/Ogg/lib/debug/win64/ogg.dll)
			copy_file_to_binaries(/External/Ogg/lib/debug/win64/libogg.dll)
			copy_file_to_binaries(/External/Vorbis/lib/debug/win64/vorbisfile.dll)
			copy_file_to_binaries(/External/Vorbis/lib/debug/win64/libvorbis.dll)
			copy_file_to_binaries(/External/Vorbis/lib/debug/win64/libvorbisfile.dll)

			copy_file_to_binaries(/External/Ogg/lib/debug/win64/libogg.lib)
			copy_file_to_binaries(/External/Vorbis/lib/debug/win64/libvorbis.lib)
			copy_file_to_binaries(/External/Vorbis/lib/debug/win64/libvorbisfile.lib)

		endif()	
	elseif(${OPIFEX_OS_WIN32})
		if(${OPIFEX_OPTION_RELEASE})

			message(STATUS "Copying... OggVorbis Win32 Release")
			copy_file_to_binaries(/External/Ogg/lib/release/win32/ogg.dll)
			copy_file_to_binaries(/External/Ogg/lib/release/win32/libogg.dll)
			copy_file_to_binaries(/External/Vorbis/lib/release/win32/vorbisfile.dll)
			copy_file_to_binaries(/External/Vorbis/lib/release/win32/libvorbis.dll)
			copy_file_to_binaries(/External/Vorbis/lib/release/win32/libvorbisfile.dll)

			copy_file_to_binaries(/External/Ogg/lib/release/win32/libogg_static.lib)
			copy_file_to_binaries(/External/Vorbis/lib/release/win32/libvorbis_static.lib)
			copy_file_to_binaries(/External/Vorbis/lib/release/win32/libvorbisfile_static.lib)

		else()


			if(${MSVC_VERSION} GREATER 1700)

				message(STATUS "Copying... OggVorbis Win32 Debug for ${MSVC_VERSION}")

				copy_file_to_binaries(/External/Ogg/lib/debug/win32/vs2013/ogg.dll)
				copy_file_to_binaries(/External/Ogg/lib/debug/win32/vs2013/libogg.dll)
				copy_file_to_binaries(/External/Vorbis/lib/debug/win32/vs2013/vorbisfile.dll)
				copy_file_to_binaries(/External/Vorbis/lib/debug/win32/vs2013/libvorbis.dll)
				copy_file_to_binaries(/External/Vorbis/lib/debug/win32/vs2013/libvorbisfile.dll)

				copy_file_to_binaries(/External/Ogg/lib/debug/win32/vs2013/libogg.lib)
				copy_file_to_binaries(/External/Vorbis/lib/debug/win32/vs2013/libvorbis.lib)
				copy_file_to_binaries(/External/Vorbis/lib/debug/win32/vs2013/libvorbisfile.lib)


			else()

				copy_file_to_binaries(/External/Ogg/lib/debug/win32/ogg.dll)
				copy_file_to_binaries(/External/Ogg/lib/debug/win32/libogg.dll)
				copy_file_to_binaries(/External/Vorbis/lib/debug/win32/vorbisfile.dll)
				copy_file_to_binaries(/External/Vorbis/lib/debug/win32/libvorbis.dll)
				copy_file_to_binaries(/External/Vorbis/lib/debug/win32/libvorbisfile.dll)

				copy_file_to_binaries(/External/Ogg/lib/debug/win32/libogg.lib)
				copy_file_to_binaries(/External/Vorbis/lib/debug/win32/libvorbis.lib)
				copy_file_to_binaries(/External/Vorbis/lib/debug/win32/libvorbisfile.lib)

			endif()

		endif()	
	endif()

endmacro(add_opifex_oggvorbis)


macro(add_opifex_steam APPLICATION_TARGET )

	if(${OPIFEX_OPTION_STEAM}) 

		message(STATUS "Link Steam")
		include_directories(../External/Steam/include/)

		message(STATUS "Steam")
		target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/Steam/lib/win32/steam_api.lib)

		copy_to_folder(${APPLICATION_TARGET} "/External/Steam/lib/" "steam_api.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})

	endif()

endmacro(add_opifex_steam)

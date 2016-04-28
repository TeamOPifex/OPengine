

macro(add_opifex_oggvorbis APPLICATION_TARGET )

	if(${OPIFEX_OPTION_AUDIO})
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
			copy_to_folder_msvc_vs(${APPLICATION_TARGET} "/External/Ogg/lib/debug/" "libogg.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
			copy_to_folder_msvc_vs(${APPLICATION_TARGET} "/External/Vorbis/lib/debug/" "libvorbis.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
			copy_to_folder_msvc_vs(${APPLICATION_TARGET} "/External/Vorbis/lib/debug/" "libvorbisfile.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
		endif()


		if(${OPIFEX_OS_WIN64})
			if(${OPIFEX_OPTION_RELEASE})
				# message(STATUS "Copying... OggVorbis Win64 Release")
				copy_file_to_binaries(/External/Ogg/lib/release/win64/ogg.dll)
				copy_file_to_binaries(/External/Ogg/lib/release/win64/libogg.dll)
				copy_file_to_binaries(/External/Vorbis/lib/release/win64/vorbisfile.dll)
				copy_file_to_binaries(/External/Vorbis/lib/release/win64/libvorbis.dll)
				copy_file_to_binaries(/External/Vorbis/lib/release/win64/libvorbisfile.dll)

				copy_file_to_binaries(/External/Ogg/lib/release/win64/libogg_static.lib)
				copy_file_to_binaries(/External/Vorbis/lib/release/win64/libvorbis_static.lib)
				copy_file_to_binaries(/External/Vorbis/lib/release/win64/libvorbisfile_static.lib)

			else()

				# message(STATUS "Copying... OggVorbis Win64 Debug")
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

				# message(STATUS "Copying... OggVorbis Win32 Release")
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

					# message(STATUS "Copying... OggVorbis Win32 Debug for ${MSVC_VERSION}")

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
	endif()
endmacro(add_opifex_oggvorbis)


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
					#target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win64/v8.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win64/v8_libbase.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win64/v8_libplatform.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win64/icuuc.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win64/icui18n.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win64/v8_base_0.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win64/v8_base_1.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win64/v8_base_2.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win64/v8_base_3.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win64/v8_nosnapshot.lib)
					#target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win64/v8_external_snapshot.lib)
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
				#copy_to_folder(${APPLICATION_TARGET} "/External/V8/lib/debug/" "icui18n.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
				#copy_to_folder(${APPLICATION_TARGET} "/External/V8/lib/debug/" "icuuc.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
				#copy_to_folder(${APPLICATION_TARGET} "/External/V8/lib/debug/" "v8.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
			endif()
		else()

			SET(CMAKE_C_FLAGS "-stdlib=libstdc++ -std=c++11"	)
			SET(CMAKE_CXX_FLAGS "-stdlib=libstdc++ -std=c++11"	)

			if(${OPIFEX_OPTION_RELEASE})

				if(${OPIFEX_OS_64})
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/release/osx64/v8.lib)
				else()

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

					target_link_libraries(${APPLICATION_TARGET}
						${PROJECT_SOURCE_DIR}/External/V8/lib/debug/osx64/libgtest.a)

					target_link_libraries(${APPLICATION_TARGET}
						${PROJECT_SOURCE_DIR}/External/V8/lib/debug/osx64/libgmock.a)

					message(STATUS "LIBV8")
				else()

				endif()

			endif()
		endif()

	endif()

endmacro(add_opifex_v8)

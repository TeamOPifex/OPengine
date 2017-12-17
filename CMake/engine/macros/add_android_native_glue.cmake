macro(add_android_native_glue)
	if(${OPIFEX_OS_ANDROID})
		# build native_app_glue as a static lib
		set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS}")
		
		add_library(native_app_glue STATIC ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

		# Export ANativeActivity_onCreate(),
		# Refer to: https://github.com/android-ndk/ndk/issues/381.
		set(CMAKE_SHARED_LINKER_FLAGS
			"${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

	endif()
endmacro(add_android_native_glue)

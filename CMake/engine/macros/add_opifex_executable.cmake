function(add_opifex_executable name)

    list(REMOVE_AT ARGV 0)

	if(${OPIFEX_OS_IOS})

		set(CMAKE_OSX_SYSROOT iphoneos2.2.1)
		set(CMAKE_OSX_ARCHITECTURES $(ARCHS_STANDARD_32_BIT))
		set(CMAKE_CXX_FLAGS "-x objective-c++")
		set(CMAKE_EXE_LINKER_FLAGS "-framework AudioToolbox -framework CoreGraphics -framework QuartzCore -framework UIKit -framework GLKit -framework OpenGLES")
		link_directories(\${HOME}/\${SDKROOT}/lib)

		set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.mycompany.\${PRODUCT_NAME:identifier}")
		set(APP_TYPE MACOSX_BUNDLE)

		add_executable(${name} ${APP_TYPE} ${ARGV})
		#set_target_properties(${NAME} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer: My Name")

		# message(STATUS "ADDING IOS EXECUTABLE ${name}")
	else()
    # message(STATUS "${OPIFEX_OPTION_RELEASE} AND ${OPIFEX_OS_WINDOWS}")
    SET(USE_AS_WINDOWS_APP ${OPIFEX_OPTION_RELEASE} AND ${OPIFEX_OS_WINDOWS})
    SET(IS_DIRECTX ${RENDER_TARGET} STREQUAL "DIRECTX_11")
    SET(USE_AS_WINDOWS_APP ${USE_AS_WINDOWS_APP} OR ${IS_DIRECTX})

    if(${USE_AS_WINDOWS_APP})
		    add_executable(${name} WIN32 ${ARGV})
        set_target_properties(${name} PROPERTIES WORKING_DIRECTORY "$(ProjectDir)$(Configuration)\\")
    else()
		    add_executable(${name} ${ARGV})
    endif()

    if(${IS_DIRECTX})
      target_link_libraries(${name} d3d11.lib d3dcompiler.lib;)
    endif()

		# message(STATUS "ADDING EXECUTABLE ${name}")
	endif()

endfunction(add_opifex_executable)

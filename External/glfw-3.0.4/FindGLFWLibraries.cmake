SET(_GLFW_GLX 1)

#--------------------------------------------------------------------
# Use X11 for window creation
#--------------------------------------------------------------------
if (${OPIFEX_OS_UNIX})

    find_package(X11 REQUIRED)

    set(GLFW_PKG_DEPS "${GLFW_PKG_DEPS} x11")

    # Set up library and include paths
    list(APPEND glfw_LIBRARIES ${X11_X11_LIB} ${CMAKE_THREAD_LIBS_INIT})
    if (UNIX AND NOT APPLE)
        list(APPEND glfw_LIBRARIES ${RT_LIBRARY})
    endif()

    # Check for XRandR (modern resolution switching and gamma control)
    if (NOT X11_Xrandr_FOUND)
        message(FATAL_ERROR "The RandR library and headers were not found")
    endif()

    list(APPEND glfw_INCLUDE_DIRS ${X11_Xrandr_INCLUDE_PATH})
    list(APPEND glfw_LIBRARIES ${X11_Xrandr_LIB})
    set(GLFW_PKG_DEPS "${GLFW_PKG_DEPS} xrandr")

    # Check for XInput (high-resolution cursor motion)
    if (NOT X11_Xinput_FOUND)
        message(FATAL_ERROR "The XInput library and headers were not found")
    endif()

    list(APPEND glfw_INCLUDE_DIRS ${X11_Xinput_INCLUDE_PATH})

    if (X11_Xinput_LIB)
        list(APPEND glfw_LIBRARIES ${X11_Xinput_LIB})
    else()
        # Backwards compatibility (bug in CMake 2.8.7)
        list(APPEND glfw_LIBRARIES Xi)
    endif()
    set(GLFW_PKG_DEPS "${GLFW_PKG_DEPS} xi")

    # Check for Xf86VidMode (fallback gamma control)
    if (NOT X11_xf86vmode_FOUND)
        message(FATAL_ERROR "The Xf86VidMode library and headers were not found")
    endif()

    list(APPEND glfw_INCLUDE_DIRS ${X11_xf86vmode_INCLUDE_PATH})
    set(GLFW_PKG_DEPS "${GLFW_PKG_DEPS} xxf86vm")

    if (X11_Xxf86vm_LIB)
        list(APPEND glfw_LIBRARIES ${X11_Xxf86vm_LIB})
    else()
        # Backwards compatibility (see CMake bug 0006976)
        list(APPEND glfw_LIBRARIES Xxf86vm)
    endif()

    # Check for Xkb (X keyboard extension)
    if (NOT X11_Xkb_FOUND)
        message(FATAL_ERROR "The X keyboard extension headers were not found")
    endif() 

    list(APPEND glfw_INCLUDE_DIR ${X11_Xkb_INCLUDE_PATH})

    find_library(RT_LIBRARY rt)
    mark_as_advanced(RT_LIBRARY)
    if (RT_LIBRARY)
        list(APPEND glfw_LIBRARIES ${RT_LIBRARY})
        set(GLFW_PKG_LIBS "${GLFW_PKG_LIBS} -lrt")
    endif()

    find_library(MATH_LIBRARY m)
    mark_as_advanced(MATH_LIBRARY)
    if (MATH_LIBRARY)
        list(APPEND glfw_LIBRARIES ${MATH_LIBRARY})
        set(GLFW_PKG_LIBS "${GLFW_PKG_LIBS} -lm")
    endif()

endif()

#--------------------------------------------------------------------
# Use GLX for context creation    # OpenGL Desktop
#--------------------------------------------------------------------
if (_GLFW_GLX)

    list(APPEND glfw_INCLUDE_DIRS ${OPENGL_INCLUDE_DIR})
    list(APPEND glfw_LIBRARIES ${OPENGL_gl_LIBRARY})

    set(GLFW_PKG_DEPS "${GLFW_PKG_DEPS} gl")

    include(CheckFunctionExists)

    set(CMAKE_REQUIRED_LIBRARIES ${OPENGL_gl_LIBRARY})
    check_function_exists(glXGetProcAddress _GLFW_HAS_GLXGETPROCADDRESS)
    check_function_exists(glXGetProcAddressARB _GLFW_HAS_GLXGETPROCADDRESSARB)
    check_function_exists(glXGetProcAddressEXT _GLFW_HAS_GLXGETPROCADDRESSEXT)

    if (NOT _GLFW_HAS_GLXGETPROCADDRESS AND
        NOT _GLFW_HAS_GLXGETPROCADDRESSARB AND
        NOT _GLFW_HAS_GLXGETPROCADDRESSEXT)
        message(WARNING "No glXGetProcAddressXXX variant found")

        # Check for dlopen support as a fallback

        set(CMAKE_REQUIRED_LIBRARIES ${CMAKE_DL_LIBS})
        check_function_exists(dlopen _GLFW_HAS_DLOPEN)
        if (NOT _GLFW_HAS_DLOPEN)
            message(FATAL_ERROR "No entry point retrieval mechanism found")
        endif()

        if (CMAKE_DL_LIBS)
            list(APPEND glfw_LIBRARIES ${CMAKE_DL_LIBS})
            set(GLFW_PKG_LIBS "${GLFW_PKG_LIBS} -l${CMAKE_DL_LIBS}")
        endif()
    endif()

endif()

#--------------------------------------------------------------------
# Use EGL for context creation   # OpenGL ES
#--------------------------------------------------------------------
if (_GLFW_EGL)

    list(APPEND glfw_INCLUDE_DIRS ${EGL_INCLUDE_DIR})
    list(APPEND glfw_LIBRARIES ${EGL_LIBRARY})

    if (UNIX)
        set(GLFW_PKG_DEPS "${GLFW_PKG_DEPS} egl")
    endif()

    if (_GLFW_USE_OPENGL)
        list(APPEND glfw_LIBRARIES ${OPENGL_gl_LIBRARY})
        list(APPEND glfw_INCLUDE_DIRS ${OPENGL_INCLUDE_DIR})
        set(GLFW_PKG_DEPS "${GLFW_PKG_DEPS} gl")
    elseif (_GLFW_USE_GLESV1)
        list(APPEND glfw_LIBRARIES ${GLESv1_LIBRARY})
        list(APPEND glfw_INCLUDE_DIRS ${GLESv1_INCLUDE_DIR})
        set(GLFW_PKG_DEPS "${GLFW_PKG_DEPS} glesv1_cm")
    elseif (_GLFW_USE_GLESV2)
        list(APPEND glfw_LIBRARIES ${GLESv2_LIBRARY})
        list(APPEND glfw_INCLUDE_DIRS ${GLESv2_INCLUDE_DIR})
        set(GLFW_PKG_DEPS "${GLFW_PKG_DEPS} glesv2")
    endif()

endif()

#--------------------------------------------------------------------
# Use Cocoa for window creation and NSOpenGL for context creation
#--------------------------------------------------------------------
if (${OPIFEX_OS_OSX})
        
    # Set up library and include paths
    find_library(COCOA_FRAMEWORK Cocoa)
    find_library(IOKIT_FRAMEWORK IOKit)
    find_library(CORE_FOUNDATION_FRAMEWORK CoreFoundation)
    find_library(CORE_VIDEO_FRAMEWORK CoreVideo)
    list(APPEND glfw_LIBRARIES ${COCOA_FRAMEWORK}
                               ${OPENGL_gl_LIBRARY}
                               ${IOKIT_FRAMEWORK}
                               ${CORE_FOUNDATION_FRAMEWORK}
                               ${CORE_VIDEO_FRAMEWORK})

    set(GLFW_PKG_DEPS "")
    set(GLFW_PKG_LIBS "-framework Cocoa -framework OpenGL -framework IOKit -framework CoreFoundation -framework CoreVideo")
endif()

#--------------------------------------------------------------------
# Export GLFW library dependencies
#--------------------------------------------------------------------
set(GLFW_LIBRARIES ${glfw_LIBRARIES} CACHE STRING "Dependencies of GLFW")
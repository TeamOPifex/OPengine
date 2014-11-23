#--------------------------------------------------------------------
# Use X11 for window creation
#--------------------------------------------------------------------
if (${OPIFEX_OS_LINUX})

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

    list(APPEND glfw_LIBRARIES ${X11_Xrandr_LIB})
    set(GLFW_PKG_DEPS "${GLFW_PKG_DEPS} xrandr")

    # Check for XInput (high-resolution cursor motion)
    if (NOT X11_Xinput_FOUND)
        message(FATAL_ERROR "The XInput library and headers were not found")
    endif()

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
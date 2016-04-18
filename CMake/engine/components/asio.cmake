
macro(add_opifex_asio_set_source)

	# Look for an External Path to the PhysX folder
	SET(_ASIO_SOURCE 0)
	if(NOT "${ASIO_PATH}" STREQUAL "")
		# message(STATUS "ASIO PATH WAS SET ${ASIO_PATH}")
		SET(_ASIO_SOURCE 1)
		SET(_ASIO_PATH "${ASIO_PATH}")
	else()
		# message(STATUS "ASIO PATH WAS NOT SET ${ASIO_PATH}")
		SET(_ASIO_PATH "${PROJECT_SOURCE_DIR}/External/ASIO/")
	endif()

endmacro(add_opifex_asio_set_source)

macro(add_opifex_asio_include)

	add_opifex_asio_set_source()
	include_directories(${_ASIO_PATH}/include/)
	# message(STATUS "   ###   ASIO INCLUDE PATH ${_ASIO_PATH}include/")

endmacro(add_opifex_asio_include)


function(add_opifex_asio APPLICATION_TARGET )

    # ASIO is a header only library

endfunction(add_opifex_asio)

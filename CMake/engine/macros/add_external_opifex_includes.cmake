

macro(add_external_opifex_includes)
	set_binary_output(BINARY_FOLDER)

	include_directories(
		${OPIFEX_ENGINE_REPOSITORY}
		${OPIFEX_ENGINE_REPOSITORY}/External/glfw/include/
		${OPIFEX_ENGINE_REPOSITORY}/External/glew-1.9.0/include/
		${OPIFEX_ENGINE_REPOSITORY}/External/glm-0.9.5/
		${OPIFEX_ENGINE_REPOSITORY}/External/GLES2/
		${OPIFEX_ENGINE_REPOSITORY}/External/OpenAL/
		${OPIFEX_ENGINE_REPOSITORY}/External/Ogg/include/
		${OPIFEX_ENGINE_REPOSITORY}/External/Vorbis/include/
		${OPIFEX_ENGINE_REPOSITORY}/External/VP8/include/
		${OPIFEX_ENGINE_REPOSITORY}/External/Spine/include/
		${OPIFEX_ENGINE_REPOSITORY}/External/Mongoose/include/
		${BINARY_FOLDER}
	)

    #message(STATUS "@@@@@ @@@@@ INCLUDING HEADERS ${JANSSON_HDR_PUBLIC}")
    #message(STATUS "@@@@@ @@@@@ FROM ${BINARY_FOLDER}")

	add_opifex_physx_include()
	add_opifex_v8_include()
	add_opifex_fmod_include()
	add_opifex_assimp_include()
	add_opifex_oculus_include()
	add_opifex_asio_include()
	add_opifex_raknet_include()

	# message(STATUS "Added External OPifex includes")
endmacro(add_external_opifex_includes)

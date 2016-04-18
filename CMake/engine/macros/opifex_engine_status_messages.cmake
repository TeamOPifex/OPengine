
function(opifex_engine_status_messages)


	set_opifex_assets()

	message(STATUS "\nOPengine CMake Success\n===================================\n
		OPIFEX_REPOSITORY: ${OPIFEX_REPOSITORY}
		OPIFEX_ENGINE_REPOSITORY: ${OPIFEX_ENGINE_REPOSITORY}
		OPIFEX_BINARIES: ${OPIFEX_BINARIES}
		OPIFEX_ASSETS: ${OPIFEX_ASSETS}
		OPIFEX_OS: ${OPIFEX_OS}
		PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}
		PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}
		OPIFEX_OPTION_RELEASE: ${OPIFEX_OPTION_RELEASE}\n")

endfunction(opifex_engine_status_messages)

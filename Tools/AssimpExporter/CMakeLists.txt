cmake_minimum_required(VERSION 2.8)

if(${OPIFEX_TOOLS_ASSIMPEXPORTER})
	# message(STATUS "Including Assimp Exporter")

	include_directories(
		.
		../
	)
	add_opifex_includes()
	add_external_opifex_includes()
	add_opifex_assimp_include()

	add_source_group(/ "${PROJECT_SOURCE_DIR}/Tools/AssimpExporter" ASSIMP_SRC)
	set(ALL_LIBS
		1_Core
		2_Data
		3_Math
		4_Performance
		5_Human
	)

  add_opifex_executable(Tools_AssimpExporter ${ASSIMP_SRC})

	target_link_libraries(Tools_AssimpExporter ${ALL_LIBS})
  add_opifex_assimp(Tools_AssimpExporter)

	add_definitions(-D${OPIFEX_OS})
	if(${OPIFEX_OPTION_RELEASE})
		add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
	endif()

	SET_PROPERTY(TARGET Tools_AssimpExporter PROPERTY FOLDER "Tools")

	set_target_properties(Tools_AssimpExporter PROPERTIES LINKER_LANGUAGE CXX)
	set_target_properties(Tools_AssimpExporter PROPERTIES OUTPUT_NAME "AssimpExporter")

endif()

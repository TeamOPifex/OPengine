cmake_minimum_required(VERSION 2.8)

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

include_directories(
	../External/freetype/win32/
	../External/glfw-3.0.4/include/
	../External/glm-0.9.1/
	../External/glew-1.5.8/include/
	../External/Phsyx.3.2.1/include/
	../External/GLES2/
	../External/Ogg/include/
	../External/Vorbis/include/
	../External/OpenAL/
	.
	../
)

add_source_group(\\ "${PROJECT_SOURCE_DIR}/Tools/FontMaker" FONTMAKER_SRC)
set(ALL_LIBS
	1_Core
	2_Data
	3_Math
	4_Performance
	5_Human
)

add_executable(Tools_FontMaker ${FONTMAKER_SRC})

target_link_libraries(Tools_FontMaker
	${ALL_LIBS}
	${PROJECT_SOURCE_DIR}/External/freetype/win32/freetype.lib
)	

add_definitions(-D${OPIFEX_OS})

SET_PROPERTY(TARGET Tools_FontMaker                PROPERTY FOLDER "Tools")

set_target_properties(Tools_FontMaker PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(Tools_FontMaker PROPERTIES 
	OUTPUT_NAME "FontMaker"
)


add_source_group(\\ "${PROJECT_SOURCE_DIR}/Tools/SpriteSheetMaker" SPRITESHEETMAKER_SRC)
add_executable(Tools_SpriteSheetMaker ${SPRITESHEETMAKER_SRC})
target_link_libraries(Tools_SpriteSheetMaker ${ALL_LIBS})	
SET_PROPERTY(TARGET Tools_SpriteSheetMaker                PROPERTY FOLDER "Tools")
set_target_properties(Tools_SpriteSheetMaker PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(Tools_SpriteSheetMaker PROPERTIES 
	OUTPUT_NAME "SpriteSheetMaker"
)

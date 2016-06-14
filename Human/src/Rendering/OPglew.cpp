#include "./Human/include/Rendering/OPglew.h"
#include "./Human/include/Rendering/OpenGL.h"
#include "./Core/include/OPlog.h"

bool glewInitialized = 0;

i8 OPglewInit() {
	if (glewInitialized) return 1;
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		OPlogErr("Failed to init glew");
		return -1;
	}
	glewInitialized = true;
	return true;
}
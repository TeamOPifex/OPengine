#include "./Human/include/Platform/opengl/OPmonitorAPIGL.h"

#ifdef OPIFEX_OPENGL

#include "./Human/include/Rendering/OPmonitor.h"
#include "./Core/include/OPmemory.h"

OPmonitor* OPmonitorGLInit(OPmonitor* monitor, GLFWmonitor* glfwMonitor) {
	OPmonitorGL* monitorGL = (OPmonitorGL*)OPalloc(sizeof(OPmonitorGL));
	monitor->internalPtr = monitorGL;

	monitorGL->Handle = glfwMonitor;

	glfwGetMonitorPos(monitorGL->Handle, &monitor->X, &monitor->Y);
	glfwGetMonitorPhysicalSize(monitorGL->Handle, &monitor->WidthMM, &monitor->HeightMM);

	i32 totalModes;
	const GLFWvidmode* modes = glfwGetVideoModes(monitorGL->Handle, &totalModes);
	monitor->VideoModesCount = totalModes;
	monitor->VideoModes = (OPmonitorVideoMode*)OPalloc(sizeof(OPmonitorVideoMode) * monitor->VideoModesCount);
	for (ui32 i = 0; i < monitor->VideoModesCount; i++) {
		monitor->VideoModes[i].Width = modes[i].width;
		monitor->VideoModes[i].Height = modes[i].height;
		monitor->VideoModes[i].Red = modes[i].redBits;
		monitor->VideoModes[i].Green = modes[i].greenBits;
		monitor->VideoModes[i].Blue = modes[i].blueBits;
		monitor->VideoModes[i].RefreshRate = modes[i].refreshRate;
	}

	const GLFWvidmode* mode = glfwGetVideoMode(monitorGL->Handle);
	monitor->VideoModeCurrent.Width = mode->width;
	monitor->VideoModeCurrent.Height = mode->height;
	monitor->VideoModeCurrent.Red = mode->redBits;
	monitor->VideoModeCurrent.Green = mode->greenBits;
	monitor->VideoModeCurrent.Blue = mode->blueBits;
	monitor->VideoModeCurrent.RefreshRate = mode->refreshRate;

	return monitor;
}

OPmonitorResult OPmonitorGLGetAll() {
	glfwInit();

	OPmonitorResult result;

	int monitorCount;
	GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
	result.count = monitorCount;
	result.monitors = (OPmonitor*)OPalloc(sizeof(OPmonitor) * monitorCount);

	for (ui8 i = 0; i < monitorCount; i++) {
		OPmonitorGLInit(&result.monitors[i], monitors[i]);
	}

	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	OPmonitorGLInit(&result.primary, primaryMonitor);
	
	return result;
}

void OPmonitorAPIGLInit(OPmonitorAPI* monitor) {
	monitor->GetAll = OPmonitorGLGetAll;
}

#endif
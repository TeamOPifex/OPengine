#include "./Human/include/Rendering/OPmonitor.h"

bool OPMONITOR_SETUP = false;

ui8 OPMONITOR_COUNT = 0;
OPmonitor OPMONITOR_PRIMARY;
OPmonitor OPMONITOR_LIST[OPMONITOR_MAX];

void OPmonitor::Init(GLFWmonitor* monitor) {
	Handle = monitor;
	glfwGetMonitorPos(Handle, &X, &Y);
	glfwGetMonitorPhysicalSize(Handle, &WidthMM, &HeightMM);

	const GLFWvidmode* modes = glfwGetVideoModes(Handle, &VideoModesCount);
	VideoModes = (OPmonitorVideoMode*)OPalloc(sizeof(OPmonitorVideoMode) * VideoModesCount);
	for (ui32 i = 0; i < VideoModesCount; i++) {
		VideoModes[i].Width = modes[i].width;
		VideoModes[i].Height = modes[i].height;
		VideoModes[i].Red = modes[i].redBits;
		VideoModes[i].Green = modes[i].greenBits;
		VideoModes[i].Blue = modes[i].blueBits;
		VideoModes[i].RefreshRate = modes[i].refreshRate;
	}

	const GLFWvidmode* mode = glfwGetVideoMode(Handle);
	VideoModeCurrent.Width = mode->width;
	VideoModeCurrent.Height = mode->height;
	VideoModeCurrent.Red = mode->redBits;
	VideoModeCurrent.Green = mode->greenBits;
	VideoModeCurrent.Blue = mode->blueBits;
	VideoModeCurrent.RefreshRate = mode->refreshRate;

	OPMONITOR_SETUP = true;
}

void OPmonitorSetup() {
	int monitorCount;

	GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
	OPMONITOR_COUNT = monitorCount;
	for (ui8 i = 0; i < monitorCount & i < OPMONITOR_MAX; i++) {
		OPMONITOR_LIST[i].Init(monitors[i]);
	}

	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	OPMONITOR_PRIMARY.Init(primaryMonitor);
}
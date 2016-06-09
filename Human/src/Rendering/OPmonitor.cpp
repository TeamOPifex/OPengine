#include "./Human/include/Rendering/OPmonitor.h"

ui8 OPMONITOR_COUNT = 0;
OPmonitor OPMONITOR_PRIMARY;
OPmonitor OPMONITOR_LIST[OPMONITOR_MAX];


void OPmonitor::Init(GLFWmonitor* monitor) {
	Monitor = monitor;
	glfwGetMonitorPos(Monitor, &X, &Y);
	glfwGetMonitorPhysicalSize(Monitor, &WidthMM, &HeightMM);

	const GLFWvidmode* modes = glfwGetVideoModes(Monitor, &VideoModesCount);
	VideoModes = (OPmonitorVideoMode*)OPalloc(sizeof(OPmonitorVideoMode) * VideoModesCount);
	for (ui32 i = 0; i < VideoModesCount; i++) {
		VideoModes[i].Width = modes[i].width;
		VideoModes[i].Height = modes[i].height;
		VideoModes[i].Red = modes[i].redBits;
		VideoModes[i].Green = modes[i].greenBits;
		VideoModes[i].Blue = modes[i].blueBits;
		VideoModes[i].RefreshRate = modes[i].refreshRate;
	}

	const GLFWvidmode* mode = glfwGetVideoMode(Monitor);
	VideoModeCurrent.Width = mode->width;
	VideoModeCurrent.Height = mode->height;
	VideoModeCurrent.Red = mode->redBits;
	VideoModeCurrent.Green = mode->greenBits;
	VideoModeCurrent.Blue = mode->blueBits;
	VideoModeCurrent.RefreshRate = mode->refreshRate;

}
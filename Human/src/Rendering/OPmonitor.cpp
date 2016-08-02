#include "./Human/include/Rendering/OPmonitor.h"
#include "./Human/include/Rendering/OPrender.h"

OPmonitorResult OPmonitor::GetAll() {
	return OPRENDERER_ACTIVE->Monitor.GetAll();
}
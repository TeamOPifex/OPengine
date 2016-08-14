#include "./Core/include/OPcore.h"

OPtimer OPTIMER;
OPchar* OPSTARTUP_PATH = NULL;
OPchar* OPEXECUTABLE_PATH = NULL;
bool OPENGINERUNNING = false;

void OPend(){
	OPENGINERUNNING = false;
}
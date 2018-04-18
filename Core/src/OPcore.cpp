#include "./Core/include/OPcore.h"

OPtimer OPTIMER;
OPtimer OPCLOCK;
OPchar* OPSTARTUP_PATH = NULL;
OPchar* OPEXECUTABLE_PATH = NULL;
bool OPENGINERUNNING = false;

void OPend(){
	OPENGINERUNNING = false;
}
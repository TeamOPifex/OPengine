#include "./Communication/include/OPnetworkState.h"
#include "./Core/include/OPmemory.h"

OPnetworkState* ActiveNetworkState = NULL;

//void OPnetworkState::Change(OPnetworkState* targetState){
//	OPnetworkState* lastState;
//
//	if(ActiveNetworkState != NULL && ActiveNetworkState->Exit(targetState)) {
//		return;
//	}	
//
//	lastState = ActiveNetworkState;
//	ActiveNetworkState = targetState;
//	ActiveNetworkState->Init(lastState);
//}

OPint OPnetworkState::Destroy(OPnetworkState* state){
	OPfree(state);
	return 1;
}

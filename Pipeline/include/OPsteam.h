// TODO: (garrett) Move this into an addon
#pragma once
//
//#include "steam_api.h"
//
//extern OPint OPSTEAM_INITIALIZED;
//
//inline OPint OPsteamInit() {
//	if(SteamAPI_Init()) {
//		OPSTEAM_INITIALIZED = 1;
//		return 1;
//	}
//	return 0;
//}
//
//inline void OPsteamRestartApp(OPint appId) {
//	SteamAPI_RestartAppIfNecessary(appId);
//}
//
//inline void OPsteamShutdown() {
//	SteamAPI_Shutdown();
//}
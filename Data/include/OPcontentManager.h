#pragma once

#ifndef OPEngine_Data_ContentMan
#define OPEngine_Data_ContentMan

#ifdef __cplusplus
extern "C"
{
#endif
#include "./Core/include/Types.h"
#include "OPhashMap.h"

OPint OPcmanInit(OPchar* assetPath);

OPint OPcmanIsLoaded(OPchar* key);
OPint OPcmanLoad(OPchar* key);
OPint OPcmanUnload(OPchar* key);


#ifdef __cplusplus
}
#endif

#endif

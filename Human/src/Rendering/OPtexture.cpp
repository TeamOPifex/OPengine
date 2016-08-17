#include "./Human/include/Rendering/OPtexture.h"
#include "./Data/include/OPcman.h"

OPtexture* OPtextureLoad(const OPchar* asset) {
    return (OPtexture*)OPCMAN.LoadGet(asset);
}

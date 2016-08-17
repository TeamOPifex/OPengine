#include "./Data/include/OPasset.h"
#include "./Data/include/OPstring.h"
#include "./Data/include/OPfile.h"

void OPasset::Init(void* asset, OPassetLoader* assetLoader, OPchar* pathToAsset) {
	this->Asset = asset;
	this->Dirty = 0;
	if (assetLoader != NULL) {
		this->Unload = assetLoader->Unload;
	}

#ifdef _DEBUG
	if (assetLoader != NULL) {
		this->Reload = assetLoader->Reload;
	}
	if (pathToAsset != NULL) {
		this->FullPath = OPstringCopy(pathToAsset);
		this->LastChange = OPfile::LastChange(FullPath);
	} else {
		this->LastChange = 0;
	}
#endif
}

void OPasset::Destroy() {
	Unload(Asset);
#ifdef _DEBUG
	OPfree(FullPath);
#endif
}
#include "./Data/include/OPasset.h"
#include "./Data/include/OPstring.h"
#include "./Data/include/OPfile.h"

void OPasset::Init(void* asset, OPassetLoader* assetLoader, OPchar* pathToAsset) {
	this->Asset = asset;
	this->Dirty = 0;
	if (assetLoader != NULL) {
		this->Unload = assetLoader->Unload;
	}
	else {
		this->Unload = NULL;
	}

#ifdef _DEBUG
	if (assetLoader != NULL) {
		this->Reload = assetLoader->Reload;
	}
	else {
		this->Reload = NULL;
	}

	this->Name = NULL;

	if (pathToAsset != NULL) {
		this->FullPath = OPstringCopy(pathToAsset);
		this->LastChange = OPfile::LastChange(FullPath);
	} else {
		this->FullPath = NULL;
		this->LastChange = 0;
	}
#endif
}

void OPasset::Destroy() {
	// Some assets may not have an unload if they were manually added to the CMAN
	// For Example: OPspriteSheet adds each of its sprites to the CMAN
	if (Unload != NULL) {
		Unload(Asset);
	}
#ifdef _DEBUG
	if (FullPath != NULL) {
		OPfree(FullPath);
	}
#endif
}
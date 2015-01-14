#pragma once
#include "./Engine.h"
#include "fbxsdk.h"

typedef struct {
	FbxScene* Scene;
	FbxNode* RootNode;
} OPfbxScene;

typedef struct {
	FbxManager* SdkManager;
	FbxIOSettings* IOSettings;
} OPfbxSdk;

OPfbxSdk OPfbxSdkCreate() {
	OPfbxSdk result;

	// Create the FBX Manager for use throughout the importer
	result.SdkManager = FbxManager::Create();
	result.IOSettings = FbxIOSettings::Create(result.SdkManager, IOSROOT);
	result.SdkManager->SetIOSettings(result.IOSettings);

	return result;
}

OPint OPfbxSdkLoadScene(OPfbxScene* result, OPfbxSdk* sdk, const OPchar* filename) {

	// Need to create an importer to use to load the actual file
	FbxImporter* importer = FbxImporter::Create(sdk->SdkManager, "");
	if(!importer) {
		OPlogDebug("Failed to initialize importer");
		return 3;
	}
	if(!importer->Initialize(filename, -1, sdk->SdkManager->GetIOSettings())) {
		OPlogDebug("Failed to initialize file: %s", filename);
		OPlogDebug("Error from FBX: %s\n", importer->GetStatus().GetErrorString());
		return 1;
	}

	FbxScene* scene = FbxScene::Create(sdk->SdkManager, "myScene");
	importer->Import(scene);
	importer->Destroy();

	result->Scene = scene;
	result->RootNode = scene->GetRootNode();
	if(!result->RootNode) {
		OPlogDebug("Failed to get Root Node from the scene of file: %s", filename);
		return 2;
	}

	return 0; // Success
}

void OPfbxSdkDestroy(OPfbxSdk* sdk) {
	sdk->SdkManager->Destroy();
}
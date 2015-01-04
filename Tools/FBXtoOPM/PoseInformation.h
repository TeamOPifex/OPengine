//#pragma once
//
//#include "SkeletonInformation.h"
//
//void GetPoseInformation(FbxScene* scene, ModelSkeletonData* skeleton) {
//
//	int      i, j, k, lPoseCount;
//	FbxString  lName;
//
//	lPoseCount = scene->GetPoseCount();
//
//	for (i = 0; i < lPoseCount; i++)
//	{
//		FbxPose* lPose = scene->GetPose(i);
//
//		lName = lPose->GetName();
//		OPlog("Pose Name: %s", lName.Buffer());
//
//		OPlog("    Is a bind pose: %d", lPose->IsBindPose());
//
//		OPlog("    Number of items in the pose: %d", lPose->GetCount());
//
//
//		for (j = 0; j < lPose->GetCount(); j++)
//		{
//			lName = lPose->GetNodeName(j).GetCurrentName();
//			OPlog("    Item name: %s", lName.Buffer());
//
//			ModelSkeletonBone* bone = NULL;
//			OPhashMapGet(skeleton->bones, lName, (void**)&bone);
//
//			if (bone != NULL) {
//				OPlog("        Bone found");
//			}
//			else {
//				OPlog("        Bone missing");
//			}
//
//			if (!lPose->IsBindPose())
//			{
//				// Rest pose can have local matrix
//				DisplayBool("    Is local space matrix: ", lPose->IsLocalMatrix(j));
//			}
//
//			//OPlog("    Matrix value: %s", "");
//
//			FbxMatrix  lMatrix = lPose->GetMatrix(j);
//
//			if (bone != NULL) {
//				FbxVector4 lRow = lMatrix.GetRow(0);
//				//bone->bindPose.cols[0].x = lRow[0];
//				//bone->bindPose.cols[1].x = lRow[1];
//				//bone->bindPose.cols[2].x = lRow[2];
//				//bone->bindPose.cols[3].x = lRow[3];
//
//				//lRow = lMatrix.GetRow(1);
//				//bone->bindPose.cols[0].y = lRow[0];
//				//bone->bindPose.cols[1].y = lRow[1];
//				//bone->bindPose.cols[2].y = lRow[2];
//				//bone->bindPose.cols[3].y = lRow[3];
//
//				//lRow = lMatrix.GetRow(2);
//				//bone->bindPose.cols[0].z = lRow[0];
//				//bone->bindPose.cols[1].z = lRow[1];
//				//bone->bindPose.cols[2].z = lRow[2];
//				//bone->bindPose.cols[3].z = lRow[3];
//
//				//lRow = lMatrix.GetRow(3);
//				//bone->bindPose.cols[0].w = lRow[0];
//				//bone->bindPose.cols[1].w = lRow[1];
//				//bone->bindPose.cols[2].w = lRow[2];
//				//bone->bindPose.cols[3].w = lRow[3];
//			}
//			//OPlog("%s", lMatrixValue.Buffer());
//
//		}
//	}
//
//	lPoseCount = scene->GetCharacterPoseCount();
//
//	for (i = 0; i < lPoseCount; i++)
//	{
//		FbxCharacterPose* lPose = scene->GetCharacterPose(i);
//		FbxCharacter*     lCharacter = lPose->GetCharacter();
//
//		if (!lCharacter) break;
//
//		OPlog("Character Pose Name: %s", lCharacter->GetName());
//
//		FbxCharacterLink lCharacterLink;
//		FbxCharacter::ENodeId  lNodeId = FbxCharacter::eHips;
//
//		while (lCharacter->GetCharacterLink(lNodeId, &lCharacterLink))
//		{
//			FbxAMatrix& lGlobalPosition = lCharacterLink.mNode->EvaluateGlobalTransform(FBXSDK_TIME_ZERO);
//
//			//OPlog("    Matrix value: %s", "");
//
//			FbxString lMatrixValue;
//
//			for (k = 0; k<4; k++)
//			{
//				FbxVector4 lRow = lGlobalPosition.GetRow(k);
//				char        lRowValue[1024];
//
//				FBXSDK_sprintf(lRowValue, 1024, "%9.4f %9.4f %9.4f %9.4f\n", lRow[0], lRow[1], lRow[2], lRow[3]);
//				lMatrixValue += FbxString("        ") + FbxString(lRowValue);
//			}
//
//			//OPlog("%s", lMatrixValue.Buffer());
//
//			lNodeId = FbxCharacter::ENodeId(int(lNodeId) + 1);
//		}
//	}
//}
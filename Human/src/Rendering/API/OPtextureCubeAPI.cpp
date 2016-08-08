#include "./Human/include/Rendering/API/OPtextureCubeAPI.h"
#include "./Human/include/Rendering/OPimage.h"
#include "./Human/include/Utilities/OPimagePNG.h"

OPtextureCube* OPtextureCubeAPI::Create(const OPchar** textures) {
	OPimage images[6];
	images[0] = OPimagePNGLoadData(textures[0]);
	images[1] = OPimagePNGLoadData(textures[1]);
	images[2] = OPimagePNGLoadData(textures[2]);
	images[3] = OPimagePNGLoadData(textures[3]);
	images[4] = OPimagePNGLoadData(textures[4]);
	images[5] = OPimagePNGLoadData(textures[5]);
	return _Create(images);
}

OPtextureCube* OPtextureCubeAPI::Init(OPtextureCube* textureCube, const OPchar** textures) {
	OPimage images[6];
	images[0] = OPimagePNGLoadData(textures[0]);
	images[1] = OPimagePNGLoadData(textures[1]);
	images[2] = OPimagePNGLoadData(textures[2]);
	images[3] = OPimagePNGLoadData(textures[3]);
	images[4] = OPimagePNGLoadData(textures[4]);
	images[5] = OPimagePNGLoadData(textures[5]);
	return _Init(textureCube, images);
}
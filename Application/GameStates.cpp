#include "GameStates.h"
#include "./Human/include/Utilities/ImagePNG.h"
#include "./Human/include/Systems/RenderSystem.h"
#include "./Human/include/Systems/FontSystem.h"
#include "./Human/include/Systems/InputSystem.h"
#include "./Data/include/ContentManager.h"
#include "./Core/include/Log.h"
#include "./Human/include/Input/Myo.h"

OPfloat t = 0;

OPfloat vertData[] = {
	 0.5,  0.5, 0,
	   1,  0,   1,
	-0.5,  0.5, 0,
	   1,  0,   0,
	-0.5, -0.5, 0,
	   0,  1,   0,
	 0.5, -0.5, 0,
	   0,  0,   1
};

ui16 indexData[] = {
	0, 2, 3, 
	2, 0, 1
};

OPmesh quadMesh;

OPmeshPacked quad;
OPmeshPacker packer;
OPmesh unPackedQuad;
OPmesh* plane;
OPeffect tri, post;
OPcam camera;
OPtexture* tex, *spec, *norm;
OPframeBuffer rt;
OPint PackerCreated = 0;
OPeffect eftTexScreenSpriteSheet;
OPtexture* spriteSheet;
OPmesh fontText;
OPfont* font;
OPtexture* fontTexture;
OPfontManager* fontManager;

void* garbage;

OPgameState State0 = {
	State0Enter,
	State0Update,
	State0Exit
};

OPgameState State1 = {
	State1Enter,
	State1Update,
	State1Exit
};

void State0Enter(OPgameState* last){
	OPlog("State0 Entering...");
	OPcmanLoad("impact.wav");
	OPcmanLoad("boom.wav");
	OPcmanLoad("background.ogg");
	OPcmanLoad("TexturedSpecular.vert");
	OPcmanLoad("TexturedSpecular.frag");
	OPcmanLoad("TexturedScreen.vert");
	OPcmanLoad("SpriteSheet.frag");
	OPcmanLoad("Font.frag");
	OPlog("Font Frag Loaded");
	OPcmanLoad("Textured.frag");
	OPcmanLoad("BiPlane.opm");
	OPcmanLoad("steamPlaneSkin.png");
	OPcmanLoad("steamPlaneSpec.png");
	OPcmanLoad("noneNorm.png");
	OPcmanLoad("stencil.opf");
	// Required
	i8** text = (i8**)OPalloc(sizeof(i8) * 1);
	text[0] = "All of the text! Woot!";
	fontManager = OPfontManagerSetup("stencil.opf", text, 1);

	// Optional
	OPfontManagerSetRGBA(fontManager, 0.0f, 0.0f, 1.0f, 1.0f);
	OPfontManagerSetAlign(fontManager, OPFONT_ALIGN_CENTER);

	OPlog("State0 Entered!");
}

ui32 backgroundState = 0;

int State0Update(OPtimer* time){
	if(time->Elapsed > 1000) return false;
	t += 0.005f * time->Elapsed;
	OPgamePadSystemUpdate();
	OPmyoUpdate();

	if (OPmyoPose() == 2) {
		backgroundState = 2;
	}

	if (OPmyoPose() == 3) {
		backgroundState = 1;
	}

	if (OPmyoPose() == 4) {
		backgroundState = 0;
	}

	if (backgroundState == 2) {
		OPrenderClear(1.0f, 0.0f, 0.0f);
	}
	else if (backgroundState == 1) {
		OPrenderClear(0.0f, 1.0f, 0.0f);
	} else {
		OPrenderClear(0.0f, 0.0f, 0.0f);
	}

	OPvec2 pos = OPgamePadLeftThumb(OPgamePad(GamePadIndex_One));

	if(OPgamePadIsDown(OPgamePad(GamePadIndex_One), GamePad_Button_BACK)){
		OPlog("Should end");
		OPend();
	}


	// Required
	OPrenderTextXY(
		"All of the text! Woot!",
		-OPmyoYaw(),
		-OPmyoPitch()
	);

	OPrenderPresent();

	return false;
}

void State0Exit(OPgameState* next){
	OPcmanDelete("impact.wav");
	OPcmanDelete("boom.wav");
	OPcmanDelete("background.ogg");
	OPcmanDelete("TexturedSpecular.vert");
	OPcmanDelete("TexturedSpecular.frag");
	OPcmanDelete("TexturedScreen.vert");
	OPcmanDelete("Textured.frag");
	OPcmanDelete("BiPlane.opm");
	OPcmanDelete("steamPlaneSkin.png");
	OPcmanDelete("steamPlaneSpec.png");
	OPcmanDelete("noneNorm.png");	

	OPfree(garbage);
}
//-----------------------------------------------------------------------------
void State1Enter(OPgameState* last){
	OPlog("State1 Entering...");
	OPcmanLoad("impact.wav");
	OPcmanLoad("boom.wav");
	OPcmanLoad("background.ogg");
	OPcmanLoad("TexturedSpecular.vert");
	OPcmanLoad("TexturedSpecular.frag");
	OPcmanLoad("TexturedScreen.vert");
	OPcmanLoad("Textured.frag");
	OPcmanLoad("BiPlane.opm");
	OPcmanLoad("steamPlaneSkin.png");
	OPcmanLoad("steamPlaneSpec.png");
	OPcmanLoad("noneNorm.png");	
	OPlog("State1 assets loaded!");

	OPcmanPurge();

	plane = (OPmesh*)OPcmanGet("BiPlane.opm");
	tex  = (OPtexture*)OPcmanGet("steamPlaneSkin.png");
	spec = (OPtexture*)OPcmanGet("steamPlaneSpec.png");
	norm = (OPtexture*)OPcmanGet("noneNorm.png");

	garbage = OPalloc(1024 * 10); // allocate ten megs of crap

	t = 0;

	OPlog("State1 Entered!");
}

int State1Update(OPtimer* time){
	if(time->Elapsed > 1000) return false;

	t += 0.005f * time->Elapsed;
	
	OPmat4 world, view, proj;
	//world = OPmat4();
	//view = OPmat4();
	//proj = OPmat4();

	OPmat4buildRotX(&world, t);
	OPcamGetView(camera, &view);
	OPcamGetProj(camera, &proj);

	OPmeshPackerBind(&packer);
	OPrenderBindMesh(plane);
	OPrenderBindEffect(&tri);

	OPtextureBind(tex);
	OPrenderParami("uColorTexture", tex->Handle);
	OPtextureBind(spec);
	OPrenderParami("uSpecularTexture", spec->Handle);
	OPtextureBind(norm);
	OPrenderParami("uNormalTexture", norm->Handle);
	OPrenderParamMat4v("uWorld", 1, &world);
	OPrenderParamMat4v("uProj", 1, &proj);
	OPrenderParamMat4v("uView", 1, &view);

	//OPframeBufferBind(&rt);
	
	OPgamePadController* _gamePad = OPgamePad(GamePadIndex_One);
	OPgamePadUpdate(_gamePad);
	
	if(OPgamePadIsConnected(_gamePad)) {
		if(OPgamePadIsDown(_gamePad, GamePad_Button_A) || OPgamePadIsDown(_gamePad, GamePad_Button_B) || OPgamePadIsDown(_gamePad, GamePad_Button_X) || OPgamePadIsDown(_gamePad, GamePad_Button_Y)) {
			OPrenderClear( 0.0f, 0.0f, 1.0f);
		} else {
			OPrenderClear( 0.0f, 0.0f, 0.0f);
		}
	} else {
		OPrenderClear(1.0f, 1.0f, 1.0f);
	}

	OPrenderMesh();
	//OPframeBufferUnbind();
	//
	//	OPrenderClear(1.0f, 1.0f, 1.0f);
	//OPrenderSetViewport(0, 0, OPrenderWidth, OPrenderHeight);

	//OPmeshPackerBind(&packer);
	//OPrenderBindEffect(&post);
	//OPmat4identity(&world);
	//OPrenderParamMat4v("uWorld", 1, &world);

	//OPtextureBind(&rt.Texture);
	//OPrenderParami("uTexture", rt.Texture.Handle);

	//OPrenderMeshPacked(&quad);


	if(t > 6) {
		//exit(0);
		OPgameStateChange(&State0);
	}


	if(OPgamePadIsConnected(_gamePad) && OPgamePadWasPressed(_gamePad, GamePad_Button_RIGHT_SHOULDER)){
		return true;
	}

	OPrenderPresent();

	return false;
}

void State1Exit(OPgameState* next){
	OPcmanDelete("impact.wav");
	OPcmanDelete("boom.wav");
	OPcmanDelete("background.ogg");
	OPcmanDelete("TexturedSpecular.vert");
	OPcmanDelete("TexturedSpecular.frag");
	OPcmanDelete("TexturedScreen.vert");
	OPcmanDelete("Textured.frag");
	OPcmanDelete("BiPlane.opm");
	OPcmanDelete("steamPlaneSkin.png");
	OPcmanDelete("steamPlaneSpec.png");
	OPcmanDelete("noneNorm.png");	

	OPrenderUnloadEffect(&tri);
	OPrenderUnloadEffect(&post);

	OPfree(garbage);
}

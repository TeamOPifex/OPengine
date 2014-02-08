#include "GameStates.h"
#include "./Data/include/OPcontentManager.h"
#include "./Math/include/Matrix4.h"
#include "./Math/include/Solvers.h"
#include "./Human/include/Utilities/ImagePNG.h"
#include "./Human/include/Utilities/Errors.h"
#include "./Human/include/Rendering/OPbuffer.h"
#include "./Human/include/Rendering/OPmesh.h"
#include "./Human/include/Rendering/OPmeshPacked.h"
#include "./Human/include/Rendering/OPmeshPacker.h"
#include "./Human/include/Rendering/OPframeBuffer.h"
#include "./Human/include/Rendering/OPeffect.h"
#include "./Human/include/Rendering/OPcamera.h"
#include "./Human/include/Rendering/OPquad.h"
#include "./Core/include/Log.h"
#include "./Performance/include/OPthread.h"
#include "./Human/include/Input/GamePadSystem.h"
#include "./Human/include/Input/Input.h"
#include "./Human/include/Rendering/OPfont.h"
#include "./Human/include/Rendering/OPfontAtlas.h"
#include "./Human/include/Rendering/Common.h"
#include "./Human/include/Rendering/OPfontManager.h"

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
	OPLog("State0 Entering...");
	OPcmanLoad("impact.wav");
	OPcmanLoad("boom.wav");
	OPcmanLoad("background.ogg");
	OPcmanLoad("TexturedSpecular.vert");
	OPcmanLoad("TexturedSpecular.frag");
	OPcmanLoad("TexturedScreen.vert");
	OPcmanLoad("SpriteSheet.frag");
	OPcmanLoad("Font.frag");
	OPLog("Font Frag Loaded");
	OPcmanLoad("Textured.frag");
	OPcmanLoad("BiPlane.opm");
	OPcmanLoad("steamPlaneSkin.png");
	OPcmanLoad("steamPlaneSpec.png");
	OPcmanLoad("noneNorm.png");
	OPcmanLoad("stencil.opf");

	//OPLog("State0 Content loaded.");

	//garbage = OPalloc(1024 * 10); // allocate ten megs of crap

	//OPcmanPurge();

	//if(!PackerCreated){
	//	packer = OPmeshPackerCreate();
	//	OPmeshPackerBind(&packer);
	//	quad = OPquadCreatePacked();
	//	OPmeshPackerBuild();
	//	PackerCreated = 1;

	//	OPtextureDescription desc = {
	//		512, 512,
	//		GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE,
	//		OPtextureLinear, OPtextureLinear,
	//		OPtextureClamp, OPtextureClamp
	//	};

	//	rt = OPframeBufferCreate(desc);
	//}

	//OPLog("GameManager::GameManager - Ready to load model");
	//plane = (OPmesh*)OPcmanGet("BiPlane.opm");
	//tex = (OPtexture*)OPcmanGet("steamPlaneSkin.png");
	//spec = (OPtexture*)OPcmanGet("steamPlaneSpec.png");
	//norm = (OPtexture*)OPcmanGet("noneNorm.png");

	//OPvec3 pos = {0, 5, 15.0f};
	//OPvec3 look = {0, 0, 0};
	//OPvec3 up = {0, 1, 0};
	unPackedQuad = OPquadCreate();

	//camera = OPcamProj(
	//	pos,
	//	look,
	//	up,
	//	0.1f,
	//	100.0f,
	//	90,
	//	(OPrenderWidth / (OPfloat)OPrenderHeight)
	//);

	//{
	//	OPshaderAttribute attribs[] = {
	//		{"aPosition",GL_FLOAT,3},
	//		{"aNormal",GL_FLOAT,3},
	//		{"aTangent",GL_FLOAT,3},
	//		{"aUV",GL_FLOAT,2}
	//	};
	//	
	//	OPLog("GameManager::GameManager - Ready to load shaders");
	//	tri = OPrenderCreateEffect(
	//		*(OPshader*)OPcmanGet("TexturedSpecular.vert"),
	//		*(OPshader*)OPcmanGet("TexturedSpecular.frag"),
	//		attribs,
	//		4
	//	);
	//}

	//{
	//	OPshaderAttribute attribs[] = {
	//		{"aPosition",GL_FLOAT,3},
	//		{"aUV",GL_FLOAT,2}
	//	};
	//	
	//	OPLog("GameManager::GameManager - Ready to load shaders");
	//	post = OPrenderCreateEffect(
	//		*(OPshader*)OPcmanGet("TexturedScreen.vert"),
	//		*(OPshader*)OPcmanGet("Textured.frag"),
	//		attribs,
	//		2
	//	);
	//}

	//t = 0;
	//		"ModelEffect"

	//OPvec2 line1[] = {
	//	{-4, 0},
	//	{ -2,  2}
	//};

	//OPvec2 line2[] = {
	//	{ 1, -1},
	//	{ -1,  1}
	//};

	//OPvec2 inter = {0, 0};

	//OPvec2 zero = {0,0};

	//OPint result = OPsolveIntersect(&line1[0], &line1[1], &line2[0], &line2[1], &inter);
	//result = OPsolveCircleIntersect(&line2[0], &line2[1], &zero, 1, &inter); 




	//i8* font_cache = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

	//font = (OPfont*)OPcmanGet("stencil.opf");
	//fontManager = OPfontManagerCreate(font);
	//OPfontManagerBind(fontManager);
	//OPfontManagerAddText("Pause");
	//OPfontManagerBuild();
	//OPcommonLoadFontEffect();

	// Required
	i8** text = (i8**)OPalloc(sizeof(i8) * 1);
	text[0] = "All of the text! Woot!";
	fontManager = OPfontManagerSetup("stencil.opf", text, 1);

	// Optional
	OPfontManagerSetRGBA(fontManager, 0.0f, 0.0f, 1.0f, 1.0f);
	OPfontManagerSetAlign(fontManager, OPFONT_ALIGN_CENTER);


	//spriteSheet = OPfontAtlasTexture(font->atlas);
	//OPshaderAttribute attribs[] = {
	//	{ "aPosition", GL_FLOAT, 3 },
	//	{ "aUV", GL_FLOAT, 2 }
	//};

	//eftTexScreenSpriteSheet = OPrenderCreateEffect(
	//	*(OPshader*)OPcmanGet("TexturedScreen.vert"),
	//	*(OPshader*)OPcmanGet("Font.frag"),
	//	attribs,
	//	2,
	//		"BlurEffect"
	//	);
	//quadMesh = OPquadCreate();

	//OPvec4 color = { 1, 0, 0, 1 };
	//OPvec2 pos2 = { 0, 0 };
	//fontText = OPfontCreateText(font, "Testing the fonts!", &color, &pos2, 0);
	//
	OPLog("State0 Entered!");
}

int State0Update(OPtimer* time){
	if(time->Elapsed > 1000) return false;
	t += 0.005f * time->Elapsed;

	//OPmat4 world, view, proj;
	//world = OPmat4();
	//view = OPmat4();
	//proj = OPmat4();

	//OPmat4buildRotY(&world, t);
	//OPcamGetView(camera, &view);
	//OPcamGetProj(camera, &proj);

	//OPLog("Binding meshes and effects");
	//OPmeshPackerBind(&packer);
	//OPrenderBindMesh(plane);
	//OPrenderBindEffect(&tri);
	//OPLog("Done binding");

	//OPLog("Binding textures...");
	//OPtextureBind(tex);
	//OPrenderParami("uColorTexture", tex->Handle);
	//OPtextureBind(spec);
	//OPrenderParami("uSpecularTexture", spec->Handle);
	//OPtextureBind(norm);
	//OPrenderParami("uNormalTexture", norm->Handle);
	//OPrenderParamMat4v("uWorld", 1, &world);
	//OPrenderParamMat4v("uProj", 1, &proj);
	//OPrenderParamMat4v("uView", 1, &view);
	//OPLog("Done binding");

	//OPmat4buildRotY(&world, t);
	//OPcamGetView(camera, &view);
	//OPcamGetProj(camera, &proj);

	//OPmeshPackerBind(&packer);
	//OPrenderBindMesh(plane);
	//OPrenderBindEffect(&tri);

	//OPtextureBind(tex);
	//OPrenderParami("uColorTexture", tex->Handle);
	//OPtextureBind(spec);
	//OPrenderParami("uSpecularTexture", spec->Handle);
	//OPtextureBind(norm);
	//OPrenderParami("uNormalTexture", norm->Handle);
	//OPrenderParamMat4v("uWorld", 1, &world);
	//OPrenderParamMat4v("uProj", 1, &proj);
	//OPrenderParamMat4v("uView", 1, &view);

	//OPframeBufferBind(&rt);
	//
	//GamePadController* gamePad = OPgamePadController(GamePadIndex_One);
	//OPgamePadUpdate(gamePad);
	//
	//if(OPgamePadIsConnected(gamePad)) {
	//	if(OPgamePadIsDown(gamePad, GamePad_Button_A) || OPgamePadIsDown(gamePad, GamePad_Button_B) || OPgamePadIsDown(gamePad, GamePad_Button_X) || OPgamePadIsDown(gamePad, GamePad_Button_Y)) {
	//		OPrenderClear( 0.0f, 0.0f, 1.0f);
	//	} else {
	//		OPrenderClear( 0.0f, 0.0f, 0.0f);
	//	}
	//} else {
	//	OPrenderClear(1.0f, 1.0f, 1.0f);
	//}

	//OPrenderDrawBufferIndexed(200);
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
	//
	//	OPrenderClear(1.0f, 1.0f, 1.0f);
	//OPrenderSetViewport(0, 0, OPrenderWidth, OPrenderHeight);


	//if(t > 6)
	//	OPgameStateChange(&State1);










	OPrenderClear(0.0f, 0.0f, 0.0f);

	// Required
	OPrenderTextXY("All of the text! Woot!", 0, 0);

	// Optional
	// OPrenderTextRGBAXYAlign("All of the text! Woot!", 0, 0, 1.0f, 1.0f, 0.0f, 0.0f, OPFONT_ALIGN_CENTER);
	






	OPrenderPresent();

	//OPLog("Update done");
	//if(OPgamePadIsConnected(gamePad) && OPgamePadWasPressed(gamePad, GamePad_Button_RIGHT_SHOULDER)){
	//	return true;
	//}



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
	OPLog("State1 Entering...");
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
	OPLog("State1 assets loaded!");

	OPcmanPurge();

	plane = (OPmesh*)OPcmanGet("BiPlane.opm");
	tex  = (OPtexture*)OPcmanGet("steamPlaneSkin.png");
	spec = (OPtexture*)OPcmanGet("steamPlaneSpec.png");
	norm = (OPtexture*)OPcmanGet("noneNorm.png");

	garbage = OPalloc(1024 * 10); // allocate ten megs of crap

	t = 0;

	OPLog("State1 Entered!");
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
	
	GamePadController* gamePad = OPgamePadController(GamePadIndex_One);
	OPgamePadUpdate(gamePad);
	
	if(OPgamePadIsConnected(gamePad)) {
		if(OPgamePadIsDown(gamePad, GamePad_Button_A) || OPgamePadIsDown(gamePad, GamePad_Button_B) || OPgamePadIsDown(gamePad, GamePad_Button_X) || OPgamePadIsDown(gamePad, GamePad_Button_Y)) {
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


	if(OPgamePadIsConnected(gamePad) && OPgamePadWasPressed(gamePad, GamePad_Button_RIGHT_SHOULDER)){
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

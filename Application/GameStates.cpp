#include "GameStates.h"
#include "./Core/include/Assert.h"
#include "./Human/include/Utilities/OPimagePNG.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Human/include/Systems/OPfontSystem.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Rendering/Sprite/OPspriteSheet.h"
#include "./Human/include/Systems/OPaudioSystem.h"
#include "./Data/include/OPcman.h"
#include "./Core/include/OPlog.h"
#include "./Human/include/Input/OPmyo.h"
#include "./Scripting/include/OPscript.h"
#include "./Scripting/include/OPloaderOPS.h"
#include "./Math/include/OPvec2.h"
#include "GameWebServer.h"

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
OPeffect tri, post, OPss;
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
OPaudioEmitter* sound;
OPaudioPlayer player;

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

OPvec3 color = OPVEC3_ZERO;
f32 fontPosX = 0, fontPosY = 0;

void ColorHandler(OPstream* str, void* param) {
	OPvec3* c = (OPvec3*)param;
	OPvec3 t = OPvec3Read(str);
	c->x = t.x;
	c->y = t.y;
	c->z = t.z;
}

void FontHandler(OPstream* str, void* param) {
	OPvec2 t = OPvec2Read(str);
	fontPosX = t.x;
	fontPosY = t.y;
}

void ColorSingleHandler(OPstream* str, void* param) {
	f32* p = (f32*)param;
	*p = OPreadf32(str);
}

void State0Enter(OPgameState* last){
	OPshaderAttribute attribs[] = {
		{ "aPosition", GL_FLOAT, 3 },
		{ "aUV", GL_FLOAT, 2 }
	};

	OPcmanLoad("impact.wav");
	OPcmanLoad("boom.wav");
	OPcmanLoad("background.ogg");
	OPcmanLoad("TexturedSpecular.vert");
	OPcmanLoad("TexturedSpecular.frag");
	OPcmanLoad("TexturedScreen.vert");
	OPcmanLoad("OPspriteSheet.frag");
	OPcmanLoad("OPspriteSheet.vert");
	OPcmanLoad("SpriteSheet.frag");
	OPcmanLoad("Font.frag");
	OPcmanLoad("Textured.frag");
	OPcmanLoad("BiPlane.opm");
	OPcmanLoad("steamPlaneSkin.png");
	OPcmanLoad("steamPlaneSpec.png");
	OPcmanLoad("noneNorm.png");
	OPcmanLoad("stencil.opf");
	OPcmanLoad("gripe.opss");

	// OPcmanLoad("Update.ops");
	// OPscript* scriptCode = (OPscript*)OPcmanGet("Update.ops");
	// OPscriptCompileAndRun(scriptCode);

	//OPwebServerOnKey(server, "color", ColorHandler, &color);
	//OPwebServerOnKey(server, "font", FontHandler, NULL);

	OPss = OPeffectCreate(
		*(OPshader*)OPcmanGet("OPspriteSheet.vert"),
		*(OPshader*)OPcmanGet("OPspriteSheet.frag"),
		attribs,
		2,
		"Sprite sheet effect"
	);

	// Required
	
	const OPchar** text = (const OPchar**)OPalloc(sizeof(i8) * 1);
	text[0] = "All of the text! Woot!";
	fontManager = OPfontManagerSetup("stencil.opf", text, 1);

	// Optional
	OPfontManagerSetColor(fontManager, 0.0f, 0.0f, 1.0f, 1.0f);
	OPfontManagerSetAlign(fontManager, OPFONT_ALIGN_CENTER);

	
	if(!OPAUD_CURR_PLAYER){
		OPaudInit();
		OPaudInitThread(11);
		player = OPaudPlayerCreate((OPaudioSource*)OPcmanGet("impact.wav"), 10, 0); 
		sound  = OPaudCreateEmitter((OPaudioSource*)OPcmanGet("impact.wav"), EMITTER_THREADED);
	}

	quadMesh = OPquadCreate();

	OPlog("Game State 0 Entered");
}

ui32 backgroundState = 0;

OPint State0Update(OPtimer* time){
	OPsprite* bg = (OPsprite*)OPcmanGet("gripe/walk");

	//OPwebServerQueue(server, "time", (i8*)&elapsed, sizeof(ui32));
	t += 0.005f * time->Elapsed;
	OPgamePadSystemUpdate();
	OPkeyboardUpdate(time);

	if (backgroundState == 2) {
		OPrenderClear(1.0f, 0.0f, 0.0f);
	}
	else if (backgroundState == 1) {
		OPrenderClear(0.0f, 1.0f, 0.0f);
	} else {
		OPrenderClear(color.x, color.y, color.z);
	}

	OPvec2 pos = OPgamePadLeftThumb(OPgamePadGet(OPGAMEPAD_ONE));

	if(OPkeyboardWasPressed(OPKEY_SPACE)){
		//OPlog("Should play");
		////OPaudSetEmitter(sound);

		//OPaudPlayerSet(&player);
		//OPaudPlayerVolume(0.3f);
		//OPaudPlayerPlay();
		////OPaudPlay();
		//OPlog("Should have played");

		color.x = 0;
		color.y = 0.5;
		color.z = 1.0;
		//OPwebServerQueue(server, "color3", (i8*)&color, sizeof(OPvec3));
		OPlog("Queued Color Message");
	}

	if(OPgamePadIsDown(OPgamePadGet(OPGAMEPAD_ONE), OPGAMEPADBUTTON_BACK)){
		OPlog("Should end");
		OPend();
	}
	OPmat4 world;
	OPmat4Identity(&world);
	OPrenderDepth(0);
	OPmeshBind(&quadMesh);
	OPeffectBind(&OPss);
	OPtextureClearActive();
	ui32 textureHandle = OPtextureBind(bg->Sheet);
	OPtexturePixelate();
	OPeffectParamMat4("uWorld", &world);
	OPeffectParami("uColorTexture", textureHandle);
	//OPlog("X: %f, Y: %f", bg->Frames[0].Offset.x, bg->Frames[0].Offset.y);
	OPeffectParamVec2("uOffset", &bg->Frames[0].Offset);
	OPeffectParamVec2("uSize", &bg->Frames[0].Size);
	OPmeshRender();

	// Required
	// OPfontRender(
	// 	"All of the text! Woot!",
	// 	pos.x + fontPosX,
	// 	pos.y + fontPosY
	// );

	OPrenderPresent();
	return false;
}

OPint State0Exit(OPgameState* next){
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
	return 0;
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

OPint State1Update(OPtimer* time){
	if(time->Elapsed > 1000) return false;

	t += 0.005f * time->Elapsed;
	
	OPmat4 world, view, proj;
	//world = OPmat4();
	//view = OPmat4();
	//proj = OPmat4();

	world = OPmat4RotX(t);
	OPcamGetView(camera, &view);
	OPcamGetProj(camera, &proj);

	OPmeshPackerBind(&packer);
	OPmeshBind(plane);
	OPeffectBind(&tri);

	OPtextureBind(tex);
	OPeffectParami("uColorTexture", tex->Handle);
	OPtextureBind(spec);
	OPeffectParami("uSpecularTexture", spec->Handle);
	OPtextureBind(norm);
	OPeffectParami("uNormalTexture", norm->Handle);
	OPeffectParamMat4v("uWorld", 1, &world);
	OPeffectParamMat4v("uProj", 1, &proj);
	OPeffectParamMat4v("uView", 1, &view);

	//OPframeBufferBind(&rt);
	
	OPgamePad* _gamePad = OPgamePadGet(OPGAMEPAD_ONE);
	OPgamePadUpdate(_gamePad);
	
	if(OPgamePadIsConnected(_gamePad)) {
		if(OPgamePadIsDown(_gamePad, OPGAMEPADBUTTON_A) || OPgamePadIsDown(_gamePad, OPGAMEPADBUTTON_B) || OPgamePadIsDown(_gamePad, OPGAMEPADBUTTON_X) || OPgamePadIsDown(_gamePad, OPGAMEPADBUTTON_Y)) {
			OPrenderClear( 0.0f, 0.0f, 1.0f);
		} else {
			OPrenderClear( 0.0f, 0.0f, 0.0f);
		}
	} else {
		OPrenderClear(1.0f, 1.0f, 1.0f);
	}

	OPmeshRender();
	//OPframeBufferUnbind();
	//
	//	OPrenderClear(1.0f, 1.0f, 1.0f);
	//OPrenderSetViewport(0, 0, OPrenderWidth, OPrenderHeight);

	//OPmeshPackerBind(&packer);
	//OPeffectBind(&post);
	//OPmat4Identity(&world);
	//OPeffectParamMat4v("uWorld", 1, &world);

	//OPtextureBind(&rt.Texture);
	//OPeffectParami("uTexture", rt.Texture.Handle);

	//OPmeshPackedRender(&quad);


	if(t > 6) {
		//exit(0);
		OPgameStateChange(&State0);
	}


	if(OPgamePadIsConnected(_gamePad) && OPgamePadWasPressed(_gamePad, OPGAMEPADBUTTON_RIGHT_SHOULDER)){
		return true;
	}

	OPrenderPresent();

	return false;
}

OPint State1Exit(OPgameState* next){
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

	OPeffectUnload(&tri);
	OPeffectUnload(&post);

	OPfree(garbage);
	return 0;
}

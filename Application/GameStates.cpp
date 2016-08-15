#include "GameStates.h"
#include "./Core/include/Assert.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Human/include/Systems/OPfontSystem.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Rendering/Sprite/OPspriteSheet.h"
#include "./Human/include/Systems/OPaudioSystem.h"
#include "./Data/include/OPcman.h"
#include "./Core/include/OPlog.h"
#include "./Human/include/Input/OPmyo.h"
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

OPmesh* quadMesh;

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
	State0Render,
	State0Exit
};

OPgameState State1 = {
	State1Enter,
	State1Update,
	State1Render,
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
	*p = str->F32();
}

void State0Enter(OPgameState* last){
	OPshaderAttribute attribs[] = {
		{ "aPosition", OPshaderElementType::FLOAT, 3 },
		{ "aUV", OPshaderElementType::FLOAT, 2 }
	};

	OPCMAN.Load("impact.wav");
	OPCMAN.Load("boom.wav");
	OPCMAN.Load("background.ogg");
	OPCMAN.Load("TexturedSpecular.vert");
	OPCMAN.Load("TexturedSpecular.frag");
	OPCMAN.Load("TexturedScreen.vert");
	OPCMAN.Load("OPspriteSheet.frag");
	OPCMAN.Load("OPspriteSheet.vert");
	OPCMAN.Load("SpriteSheet.frag");
	OPCMAN.Load("Font.frag");
	OPCMAN.Load("Textured.frag");
	OPCMAN.Load("BiPlane.opm");
	OPCMAN.Load("steamPlaneSkin.png");
	OPCMAN.Load("steamPlaneSpec.png");
	OPCMAN.Load("noneNorm.png");
	OPCMAN.Load("stencil.opf");
	OPCMAN.Load("gripe.opss");

	// OPCMAN.Load("Update.ops");
	// OPscript* scriptCode = (OPscript*)OPCMAN.Get("Update.ops");
	// OPscriptCompileAndRun(scriptCode);

	//OPwebServerOnKey(server, "color", ColorHandler, &color);
	//OPwebServerOnKey(server, "font", FontHandler, NULL);

	OPss.Init((OPshader*)OPCMAN.Get("OPspriteSheet.vert"), (OPshader*)OPCMAN.Get("OPspriteSheet.frag"));

	// Required
	
	const OPchar** text = (const OPchar**)OPalloc(sizeof(i8) * 1);
	text[0] = "All of the text! Woot!";
	fontManager = OPfontManager::Create("stencil.opf", text, 1);

	// Optional
	fontManager->SetColor(0.0f, 0.0f, 1.0f, 1.0f);
	fontManager->SetAlign(OPfontAlign::CENTER);

	
	if(!OPAUD_CURR_PLAYER){
		OPaudInit();
		OPaudInitThread(11);
		player = OPaudPlayerCreate((OPaudioSource*)OPCMAN.Get("impact.wav"), 10, 0); 
		sound  = OPaudCreateEmitter((OPaudioSource*)OPCMAN.Get("impact.wav"), EMITTER_THREADED);
	}

	quadMesh = OPquadCreate();

	OPlog("Game State 0 Entered");
}

ui32 backgroundState = 0;

OPint State0Update(OPtimer* time){
	OPsprite* bg = (OPsprite*)OPCMAN.Get("gripe/walk");

	//OPwebServerQueue(server, "time", (i8*)&elapsed, sizeof(ui32));
	t += 0.005f * time->Elapsed;
	OPGAMEPADS.Update();
	OPKEYBOARD.Update(time);

	if (backgroundState == 2) {
		OPrenderClear(1.0f, 0.0f, 0.0f);
	}
	else if (backgroundState == 1) {
		OPrenderClear(0.0f, 1.0f, 0.0f);
	} else {
		OPrenderClear(color.x, color.y, color.z);
	}

	OPvec2 pos = OPGAMEPADS[0]->LeftThumb();

	if(OPKEYBOARD.WasPressed(OPkeyboardKey::SPACE)){
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

	if(OPGAMEPADS[0]->IsDown(OPgamePadButton::BACK)){
		OPlog("Should end");
		OPend();
	}
	OPmat4 world;
	OPmat4Identity(&world);
	OPrenderDepth(0);
	quadMesh->Bind();
	OPss.Bind();
	OPeffectSet("uWorld", &world);
	OPeffectSet("uColorTexture", bg->Sheet, 0);
	//OPlog("X: %f, Y: %f", bg->Frames[0].Offset.x, bg->Frames[0].Offset.y);
	OPeffectSet("uOffset", &bg->Frames[0].Offset);
	OPeffectSet("uSize", &bg->Frames[0].Size);
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
void State0Render(OPfloat delta) {

}
OPint State0Exit(OPgameState* next){
	OPCMAN.Delete("impact.wav");
	OPCMAN.Delete("boom.wav");
	OPCMAN.Delete("background.ogg");
	OPCMAN.Delete("TexturedSpecular.vert");
	OPCMAN.Delete("TexturedSpecular.frag");
	OPCMAN.Delete("TexturedScreen.vert");
	OPCMAN.Delete("Textured.frag");
	OPCMAN.Delete("BiPlane.opm");
	OPCMAN.Delete("steamPlaneSkin.png");
	OPCMAN.Delete("steamPlaneSpec.png");
	OPCMAN.Delete("noneNorm.png");	

	OPfree(garbage);
	return 0;
}
//-----------------------------------------------------------------------------
void State1Enter(OPgameState* last){
	OPlog("State1 Entering...");
	OPCMAN.Load("impact.wav");
	OPCMAN.Load("boom.wav");
	OPCMAN.Load("background.ogg");
	OPCMAN.Load("TexturedSpecular.vert");
	OPCMAN.Load("TexturedSpecular.frag");
	OPCMAN.Load("TexturedScreen.vert");
	OPCMAN.Load("Textured.frag");
	OPCMAN.Load("BiPlane.opm");
	OPCMAN.Load("steamPlaneSkin.png");
	OPCMAN.Load("steamPlaneSpec.png");
	OPCMAN.Load("noneNorm.png");	
	OPlog("State1 assets loaded!");

	OPCMAN.Purge();

	plane = (OPmesh*)OPCMAN.Get("BiPlane.opm");
	tex  = (OPtexture*)OPCMAN.Get("steamPlaneSkin.png");
	spec = (OPtexture*)OPCMAN.Get("steamPlaneSpec.png");
	norm = (OPtexture*)OPCMAN.Get("noneNorm.png");

	garbage = OPalloc(1024 * 10); // allocate ten megs of crap

	t = 0;

	OPlog("State1 Entered!");
}

OPint State1Update(OPtimer* time){
	if(time->Elapsed > 1000) return false;

	t += 0.005f * time->Elapsed;
	
	OPmat4 world;
	//world = OPmat4();
	//view = OPmat4();
	//proj = OPmat4();

	world = OPmat4RotX(t);

	packer.Bind();
	plane->Bind();
	tri.Bind();

	OPeffectSet("uColorTexture", tex, 0);
	OPeffectSet("uSpecularTexture", spec, 1);
	OPeffectSet("uNormalTexture", norm, 2);
	OPeffectSet("uWorld", 1, &world);
	OPeffectSet("uProj", 1, &camera.proj);
	OPeffectSet("uView", 1, &camera.view);

	//OPframeBufferBind(&rt);
	
	OPgamePad* _gamePad = OPGAMEPADS[0];
	_gamePad->Update();
	
	if(_gamePad->IsConnected()) {
		if(_gamePad->IsDown(OPgamePadButton::A) || _gamePad->IsDown(OPgamePadButton::B) || _gamePad->IsDown(OPgamePadButton::X) || _gamePad->IsDown(OPgamePadButton::Y)) {
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
		OPgameState::Change(&State0);
	}


	if(_gamePad->IsConnected() && _gamePad->WasPressed(OPgamePadButton::RIGHT_SHOULDER)){
		return true;
	}

	OPrenderPresent();

	return false;
}
void State1Render(OPfloat delta) {

}
OPint State1Exit(OPgameState* next){
	OPCMAN.Delete("impact.wav");
	OPCMAN.Delete("boom.wav");
	OPCMAN.Delete("background.ogg");
	OPCMAN.Delete("TexturedSpecular.vert");
	OPCMAN.Delete("TexturedSpecular.frag");
	OPCMAN.Delete("TexturedScreen.vert");
	OPCMAN.Delete("Textured.frag");
	OPCMAN.Delete("BiPlane.opm");
	OPCMAN.Delete("steamPlaneSkin.png");
	OPCMAN.Delete("steamPlaneSpec.png");
	OPCMAN.Delete("noneNorm.png");	

	tri.Destroy();
	post.Destroy();

	OPfree(garbage);
	return 0;
}

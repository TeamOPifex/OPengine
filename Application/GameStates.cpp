#include "GameStates.h"
#include "./Core/include/Assert.h"
#include "./Human/include/Utilities/ImagePNG.h"
#include "./Human/include/Systems/RenderSystem.h"
#include "./Human/include/Systems/FontSystem.h"
#include "./Human/include/Systems/InputSystem.h"
#include "./Human/include/Rendering/Sprite/SpriteSheet.h"
#include "./Data/include/ContentManager.h"
#include "./Core/include/Log.h"
#include "./Human/include/Input/Myo.h"
#include "./Scripting/include/Scripting.h"
#include "./Human/include/Utilities/LoaderOPS.h"

#include "./Pipeline/include/Texture3D.h"

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
OPcam* camera;
OPtexture* tex, *spec, *norm;
OPframeBuffer rt;
OPint PackerCreated = 0;
OPeffect eftTexScreenSpriteSheet;
OPtexture* spriteSheet;
OPmesh fontText;
OPfont* font;
OPtexture* fontTexture;
OPfontManager* fontManager;
OPtexture3D* tex3d;

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

OPvec3 Camera_position = { 0.0f, 0.0f, 5.0f };
OPvec3 Camera_target = { 0.0f, 0.0f, 0.0f };
OPfloat Camera_width_adjust = 1.0f;
OPfloat Camera_height_adjust = 1.0f;
OPcam GetVersusCamera(){
	OPrenderGetWidth();

	OPfloat widthMod = OPrenderWidth / 1280.0f;
	OPfloat heightMod = OPrenderHeight / 720.0f;

	OPfloat w = 150 * widthMod * Camera_width_adjust;
	OPfloat h = 75 * heightMod * Camera_height_adjust;

	OPvec3 up = { 0.0f, 1.0f, 0.0f };

	return OPcamOrtho(Camera_position, Camera_target, up, 1.0f, 1000.0f, -w, w, h, -h);
}

void State0Enter(OPgameState* last){
	OPshaderAttribute attribs[] = {
		{ "aPosition", GL_FLOAT, 3 },
		{ "aUV", GL_FLOAT, 2 }
	};
	OPcmanLoad("tester.opss");


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

	OPss = OPrenderCreateEffect(
		*(OPshader*)OPcmanGet("OPspriteSheet.vert"),
		*(OPshader*)OPcmanGet("OPspriteSheet.frag"),
		attribs,
		2,
		"Sprite sheet effect"
	);

	// Required
	
	OPchar** text = (OPchar**)OPalloc(sizeof(i8)* 2);
	text[0] = "All of the text! Woot!";
	text[1] = "Tested"; 
	fontManager = OPfontManagerSetup("stencil.opf", text, 2);

	// Optional
	OPfontManagerSetRGBA(fontManager, 0.0f, 0.0f, 1.0f, 1.0f);
	OPfontManagerSetAlign(fontManager, OPFONT_ALIGN_CENTER);

	OPcmanLoad("Update.ops");
	OPscript* script = (OPscript*)OPcmanGet("Update.ops");
	OPscriptCompile(script);

	quadMesh = OPquadCreate();

	tex3d = OPtexture3DCreate((OPtexture*)OPcmanGet("steamPlaneSkin.png"), NULL);

	camera = (OPcam*)OPalloc(sizeof(OPcam));
	*camera = GetVersusCamera();

	tex3d->Scale = OPvec3One * 10;

	OPlog("Game State 0 Entered");
}

ui32 backgroundState = 0;
OPint swap_state = 0;
OPint frame = 0;

int State0Update(OPtimer* time) {
	OPrenderClear(0.0, 0.0, 0.0);
	OPtexture3DRender(tex3d, camera );
	OPrenderPresent();
	return false;
}

void State0Exit(OPgameState* next) {
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

#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./Pipeline/include/Rendering.h"
#include "./Pipeline/include/Renderers/OPrendererPBR.h"
#include "./Pipeline/include/OPmaterialPBR.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Human/include/Rendering/Primitives/OPcube.h"
#include "./Human/include/Rendering/Primitives/OPsphere.h"
#include "./Data/include/OPcman.h"

enum PBRStates {
	PBR_DEFAULT,
	PBR_EQUIRECTANGULAR,
	PBR_CUBEMAP,
	PBR_CUBEMAPVIEW,
	PBR_CONVOLUTE,
	PBR_HDRFILTER,
	PBR_EFFECT
};

#define PBREXAMPLE_MODELCOUNT 7

class PBRExample : public OPgameState {
	OPcamFreeFlight camera;

	// PBR_DEFAULT
	OPscene scene;
	OPrendererPBR renderer;
	OPmodel* Mesh;
	OPmodel* Mesh2;
	PBRStates pbrState = PBRStates::PBR_EFFECT;// PBR_DEFAULT;

	// PBR_EQUIRECTANGULAR
	OPmodel* cubeMesh;
	OPeffect EquirectangularEffect;
	OPtexture* Equirectangular;

	// PBR_CUBEMAP
	OPframeBuffer captureFB;
	OPtextureCube textureCube;
	OPeffect EquirectangularToCubeMapEffect;
	i32 side = -1;
	
	// PBR_CUBEMAP View
	OPeffect SkyboxEffect;
	OPtextureCube testTextureCube;

	// PBR_CONVOLUTE
	OPtextureCube convoluteCube;
	OPeffect ConvoluteEffect;

	// PBR_HDRFILTER
	OPtextureCube hdrRoughnessFilteredCube;
	OPtexture brdfTexture;
	OPtexture2DOLD* BRDFTex2D;
	OPmodel* quad;
	OPeffect BRDFEffect;

	// PBR Effect
	OPeffect pbrEffect;
	OPtexture* albedoTex;
	OPtexture* normalTex;
	OPtexture* metallicTex;
	OPtexture* roughnessTex;
	OPtexture* aoTex;
	OPtexture* albedoTex2;
	OPtexture* normalTex2;
	OPtexture* metallicTex2;
	OPtexture* roughnessTex2;
	OPtexture* aoTex2;

	OPmaterialPBR basePBRMaterial;
	struct PBRModel {
		OPmodel* model;
		OPmat4 world;
		OPtexture* albedoTex;
		OPtexture* normalTex;
		OPtexture* metallicTex;
		OPtexture* roughnessTex;
		OPtexture* aoTex;
		OPmaterial* material;
	};

	PBRModel pbrModels[PBREXAMPLE_MODELCOUNT];

	OPvec3 lightPostions[4];
	OPvec3 lightColors[4];
	OPfloat dist = 20;

	ui32 elapsedTime = 0;

	void Init(OPgameState* last) {
		camera.Init();


		lightPostions[0] = OPvec3(dist, 0.0, 0.0);
		lightPostions[1] = OPvec3(-dist, 0, dist);
		lightPostions[2] = OPvec3(-dist, 0, -dist);
		lightPostions[3] = OPvec3(dist, 0, -dist);
		lightColors[0] = OPvec3(300.0f);
		lightColors[1] = OPvec3(300.0f);
		lightColors[2] = OPvec3(300.0f);


		if (pbrState == PBRStates::PBR_DEFAULT) {
			scene.Init(&renderer, 100, 1);
			scene.SetCamera(&camera.Camera);

			SetupModels();

			for (ui32 i = 0; i < PBREXAMPLE_MODELCOUNT; i++) {
				OPrendererEntity* entity = scene.Add( pbrModels[i].model , OPrendererEntityDesc(false, true, true, false));
				entity->material->SetMap("uAlbedoMap", pbrModels[i].albedoTex);
				entity->material->SetMap("uNormalMap", pbrModels[i].normalTex);
				entity->material->SetMap("uMetallicMap", pbrModels[i].metallicTex);
				entity->material->SetMap("uRoughnessMap", pbrModels[i].roughnessTex);
				entity->material->SetMap("uAOMap", pbrModels[i].aoTex);
				entity->world = pbrModels[i].world;
			}

		}
		else if (pbrState == PBRStates::PBR_EQUIRECTANGULAR) {


			// PBRStates::PBR_EQUIRECTANGULAR
			cubeMesh = OPcubeCreate();
			EquirectangularEffect.Init("equirectangular.vert", "equirectangular.frag");
			Equirectangular = OPtexture::Load("newport_loft.hdr");
		}
		else if (pbrState == PBRStates::PBR_CUBEMAP) {
			Equirectangular = OPtexture::Load("newport_loft.hdr");
			OPtextureCube::FromEquirectangular(&textureCube, Equirectangular, 1024);
			//RenderEquirectangularToCubeMap();
		}
		else if (pbrState == PBRStates::PBR_CUBEMAPVIEW) {
			cubeMesh = OPcubeCreate();
			Equirectangular = OPtexture::Load("newport_loft.hdr");
			//RenderEquirectangularToCubeMap();

			OPtextureCube::FromEquirectangular(&textureCube, Equirectangular, 1024);

			const OPchar* images[6];
			for (ui32 i = 0; i < 6; i++) images[i] = "Textures/TetrisGreen.png";
			testTextureCube.Init(images);

			SkyboxEffect.Init("skybox.vert", "skybox.frag");
		}
		else if (pbrState == PBRStates::PBR_CONVOLUTE) {
			cubeMesh = OPcubeCreate();
			Equirectangular = OPtexture::Load("newport_loft.hdr");
			//RenderEquirectangularToCubeMap();

			//const OPchar* images[6];
			//for (ui32 i = 0; i < 6; i++) images[i] = "Textures/TetrisGreen.png";
			//testTextureCube.Init(images);


			OPtextureCube::FromEquirectangular(&textureCube, Equirectangular, 512);
			OPtextureCube::Convolute(&convoluteCube, &textureCube, 32);

			SkyboxEffect.Init("skybox.vert", "skybox.frag");
			ConvoluteEffect.Init("Convolute.vert", "Convolute.frag");
		}
		else if (pbrState == PBRStates::PBR_HDRFILTER) {
			Equirectangular = OPtexture::Load("newport_loft.hdr");
			OPtextureCube::FromEquirectangular(&textureCube, Equirectangular, 512);
			OPtextureCube::RoughnessMap(&hdrRoughnessFilteredCube, &textureCube, 128);
			SkyboxEffect.Init("skybox.vert", "skybox.frag");
			cubeMesh = OPcubeCreate();
			
			OPtexture::GenerateBRDF(&brdfTexture, 512);

			//BRDFTex2D = OPtexture2DCreate(&brdfTexture, NULL, OPVEC2_ZERO, OPvec2(1.0f, 1.0f));
			//BRDFTex2D->Scale = OPvec2(1.0f, 1.0f);
			//BRDFTex2D->Position = OPvec2(0.5f, 0.5f);

			quad = OPquadCreate(0.5, 1.0);
			//BRDFEffect.Init("BRDF.vert", "BRDF.frag");
			BRDFEffect.Init("BRDF.vert", "BRDFViewer.frag");
		}
		else if (pbrState == PBRStates::PBR_EFFECT) {
			cubeMesh = OPcubeCreate();
			
			SetEnv("newport_loft.hdr");
			OPtexture::GenerateBRDF(&brdfTexture, 512);

			SkyboxEffect.Init("skybox.vert", "skybox.frag");
			pbrEffect.Init("Common/OPpbr.vert", "Common/OPpbrIBL.frag");


			basePBRMaterial.Init();
			basePBRMaterial.AddParam("uLightPositions[0]", &lightPostions);
			basePBRMaterial.AddParam("uLightColors[0]", &lightColors);
			basePBRMaterial.AddParam("uCamPos", &camera.Camera.pos);
			basePBRMaterial.AddParam("uIrradianceMap", &convoluteCube);
			basePBRMaterial.AddParam("uPrefilterMap", &hdrRoughnessFilteredCube);
			basePBRMaterial.AddParam("uBRDFLUT", &brdfTexture);

			SetupModels();
			for (ui32 i = 0; i < PBREXAMPLE_MODELCOUNT; i++) {
				pbrModels[i].material = basePBRMaterial.CreateInstances(pbrModels[i].model, false);
				pbrModels[i].material->SetMap("uAlbedoMap", pbrModels[i].albedoTex);
				pbrModels[i].material->SetMap("uNormalMap", pbrModels[i].normalTex);
				pbrModels[i].material->SetMap("uMetallicMap", pbrModels[i].metallicTex);
				pbrModels[i].material->SetMap("uRoughnessMap", pbrModels[i].roughnessTex);
				pbrModels[i].material->SetMap("uAOMap", pbrModels[i].aoTex);
			}
		}

	}

	void SetupModels() {

		ui32 model = 0;

		// pbrModels[model].model = (OPmodel*)OPCMAN.LoadGet("Cerberus.opm");
		// pbrModels[model].albedoTex = OPtexture::Load("Cerberus_A.png");
		// pbrModels[model].normalTex = OPtexture::Load("Cerberus_N.png");
		// pbrModels[model].metallicTex = OPtexture::Load("Cerberus_M.png");
		// pbrModels[model].roughnessTex = OPtexture::Load("Cerberus_R.png");
		// pbrModels[model].aoTex = OPtexture::Load("EmptyAO.png");
		// pbrModels[model].world.SetTranslate(-10, 0, 0)->RotX(-OPpi_2)->Scl(0.1f);
		// model++;

		pbrModels[model].model = (OPmodel*)OPCMAN.LoadGet("bug.opm");
		pbrModels[model].albedoTex = OPtexture::Load("bug_albedo.png");
		pbrModels[model].normalTex = OPtexture::Load("bug_normal.png");
		pbrModels[model].metallicTex = OPtexture::Load("bug_metallic.png");
		pbrModels[model].roughnessTex = OPtexture::Load("bug_roughness.png");
		pbrModels[model].aoTex = OPtexture::Load("EmptyAO.png");
		pbrModels[model].world.SetTranslate(-10, 0, 0)->RotX(-OPpi_2)->Scl(0.1f);
		model++;


		pbrModels[model].model = (OPmodel*)OPCMAN.LoadGet("uvsphere20.opm");
		pbrModels[model].albedoTex = OPtexture::Load("gold/albedo.png");
		pbrModels[model].normalTex = OPtexture::Load("gold/normal.png");
		pbrModels[model].metallicTex = OPtexture::Load("gold/metallic.png");
		pbrModels[model].roughnessTex = OPtexture::Load("gold/roughness.png");
		pbrModels[model].aoTex = OPtexture::Load("gold/ao.png");
		pbrModels[model].world.SetScl(2.0f)->Translate(2, 0, 0);
		model++;

		pbrModels[model].model = (OPmodel*)OPCMAN.LoadGet("uvsphere20.opm");
		pbrModels[model].albedoTex = OPtexture::Load("grass/albedo.png");
		pbrModels[model].normalTex = OPtexture::Load("grass/normal.png");
		pbrModels[model].metallicTex = OPtexture::Load("grass/metallic.png");
		pbrModels[model].roughnessTex = OPtexture::Load("grass/roughness.png");
		pbrModels[model].aoTex = OPtexture::Load("grass/ao.png");
		pbrModels[model].world.SetScl(2.0f)->Translate(4, 0, 0);
		model++;

		pbrModels[model].model = (OPmodel*)OPCMAN.LoadGet("uvsphere20.opm");
		pbrModels[model].albedoTex = OPtexture::Load("plastic/albedo.png");
		pbrModels[model].normalTex = OPtexture::Load("plastic/normal.png");
		pbrModels[model].metallicTex = OPtexture::Load("plastic/metallic.png");
		pbrModels[model].roughnessTex = OPtexture::Load("plastic/roughness.png");
		pbrModels[model].aoTex = OPtexture::Load("plastic/ao.png");
		pbrModels[model].world.SetScl(2.0f)->Translate(6, 0, 0);
		model++;

		pbrModels[model].model = (OPmodel*)OPCMAN.LoadGet("uvsphere20.opm");
		pbrModels[model].albedoTex = OPtexture::Load("rusted_iron/albedo.png");
		pbrModels[model].normalTex = OPtexture::Load("rusted_iron/normal.png");
		pbrModels[model].metallicTex = OPtexture::Load("rusted_iron/metallic.png");
		pbrModels[model].roughnessTex = OPtexture::Load("rusted_iron/roughness.png");
		pbrModels[model].aoTex = OPtexture::Load("rusted_iron/ao.png");
		pbrModels[model].world.SetScl(2.0f)->Translate(8, 0, 0);
		model++;

		pbrModels[model].model = (OPmodel*)OPCMAN.LoadGet("uvsphere20.opm");
		pbrModels[model].albedoTex = OPtexture::Load("wall/albedo.png");
		pbrModels[model].normalTex = OPtexture::Load("wall/normal.png");
		pbrModels[model].metallicTex = OPtexture::Load("wall/metallic.png");
		pbrModels[model].roughnessTex = OPtexture::Load("wall/roughness.png");
		pbrModels[model].aoTex = OPtexture::Load("wall/ao.png");
		pbrModels[model].world.SetScl(2.0f)->Translate(10, 0, 0);
		model++;

		pbrModels[model].model = (OPmodel*)OPCMAN.LoadGet("MaterialBall.opm");
		pbrModels[model].albedoTex = OPtexture::Load("materialball/albedo.png");
		pbrModels[model].normalTex = OPtexture::Load("materialball/normal.png");
		pbrModels[model].metallicTex = OPtexture::Load("materialball/metallic.png");
		pbrModels[model].roughnessTex = OPtexture::Load("materialball/roughness.png");
		pbrModels[model].aoTex = OPtexture::Load("materialball/ao.png");
		pbrModels[model].world.SetTranslate(10, 0, 10)->Scl(0.5f);
		model++;
		
	}

	void SetEnv(const OPchar* t) {
		if (pbrState == PBRStates::PBR_DEFAULT) {
			renderer.SetEnv(t);
		}
		else {
			Equirectangular = OPtexture::Load(t);
			OPtextureCube::FromEquirectangular(&textureCube, Equirectangular, 1024);
			OPtextureCube::Convolute(&convoluteCube, &textureCube, 64);
			OPtextureCube::RoughnessMap(&hdrRoughnessFilteredCube, &textureCube, 256);
		}
	}

	OPint Update(OPtimer* time) {
		elapsedTime += time->Elapsed;

		camera.Update(time);
		if (OPKEYBOARD.WasPressed(OPkeyboardKey::I)) {
			side--;
			if (side < -1) {
				side = -1;
			}
		}
		if (OPKEYBOARD.WasPressed(OPkeyboardKey::K)) {
			side++;
			if (side > 5) {
				side = 5;
			}
		}

		if (OPKEYBOARD.WasPressed(OPkeyboardKey::N1)) {
			SetEnv("newport_loft.hdr");
		}
		if (OPKEYBOARD.WasPressed(OPkeyboardKey::N2)) {
			SetEnv("GCanyon_C_YumaPoint_3k.hdr");
		}
		if (OPKEYBOARD.WasPressed(OPkeyboardKey::N3)) {
			SetEnv("PaperMill_E_3k.hdr");
		}
		if (OPKEYBOARD.WasPressed(OPkeyboardKey::N4)) {
			SetEnv("Theatre-Center_2k.hdr");
		}
		if (OPKEYBOARD.WasPressed(OPkeyboardKey::N5)) {
			SetEnv("HDR_112_River_Road_2_Ref.hdr");
		}
		return false;
	}

	void Render(OPfloat delta) {
		//OPrenderClear(0.01f, 0.01f, 0.01f);
		OPrenderClear(0.2f);

		if (pbrState == PBRStates::PBR_DEFAULT) {
			scene.Render(delta);
		}
		else if (pbrState == PBRStates::PBR_EQUIRECTANGULAR) {

			OPcam views[6];
			views[0].SetPerspective(OPVEC3_ZERO, OPvec3(1, 0, 0), OPvec3(0, -1, 0), 0.1f, 10.0f, 90.0f, 1.0);
			views[1].SetPerspective(OPVEC3_ZERO, OPvec3(-1, 0, 0), OPvec3(0, -1, 0), 0.1f, 10.0f, 90.0f, 1.0);
			views[2].SetPerspective(OPVEC3_ZERO, OPvec3(0, 1, 0), OPvec3(0, 0, 1), 0.1f, 10.0f, 90.0f, 1.0);
			views[3].SetPerspective(OPVEC3_ZERO, OPvec3(0, -1, 0), OPvec3(0, 0, -1), 0.1f, 10.0f, 90.0f, 1.0);
			views[4].SetPerspective(OPVEC3_ZERO, OPvec3(0, 0, 1), OPvec3(0, -1, 0), 0.1f, 10.0f, 90.0f, 1.0);
			views[5].SetPerspective(OPVEC3_ZERO, OPvec3(0, 0, -1), OPvec3(0, -1, 0), 0.1f, 10.0f, 90.0f, 1.0);

			EquirectangularEffect.Bind();
			EquirectangularEffect.Set("uEquirectangularMap", Equirectangular, 0);
			if (side > -1) {
				OPRENDERER_ACTIVE->SetViewport(0, 0, 512, 512);
				EquirectangularEffect.Set("uProj", &views[0].proj);
				EquirectangularEffect.Set("uView", &views[side].view);
			}
			else {
				OPRENDERER_ACTIVE->SetViewport(0, 0, 1280, 720);
				EquirectangularEffect.Set("uProj", &camera.Camera.proj);
				EquirectangularEffect.Set("uView", &camera.Camera.view);
			}
			cubeMesh->Bind();
			OPrenderDrawBufferIndexed(0);

		}
		else if (pbrState == PBRStates::PBR_CUBEMAPVIEW) {

			OPRENDERER_ACTIVE->SetDepthFunc(OPdepthFunction::LEQUAL);
			SkyboxEffect.Bind();
			//SkyboxEffect.Set("uEnvironmentMap", &testTextureCube, 0);
			SkyboxEffect.Set("uEnvironmentMap", &textureCube, 0);
			SkyboxEffect.Set("uProj", &camera.Camera.proj);
			SkyboxEffect.Set("uView", &camera.Camera.view);
			cubeMesh->Bind();
			OPrenderDrawBufferIndexed(0);
		}
		else if (pbrState == PBRStates::PBR_CONVOLUTE) {

			if (side > -1) {

				OPcam views[6];
				views[0].SetPerspective(OPVEC3_ZERO, OPvec3(1, 0, 0), OPvec3(0, -1, 0), 0.1f, 10.0f, 90.0f, 1.0);
				views[1].SetPerspective(OPVEC3_ZERO, OPvec3(-1, 0, 0), OPvec3(0, -1, 0), 0.1f, 10.0f, 90.0f, 1.0);
				views[2].SetPerspective(OPVEC3_ZERO, OPvec3(0, 1, 0), OPvec3(0, 0, 1), 0.1f, 10.0f, 90.0f, 1.0);
				views[3].SetPerspective(OPVEC3_ZERO, OPvec3(0, -1, 0), OPvec3(0, 0, -1), 0.1f, 10.0f, 90.0f, 1.0);
				views[4].SetPerspective(OPVEC3_ZERO, OPvec3(0, 0, 1), OPvec3(0, -1, 0), 0.1f, 10.0f, 90.0f, 1.0);
				views[5].SetPerspective(OPVEC3_ZERO, OPvec3(0, 0, -1), OPvec3(0, -1, 0), 0.1f, 10.0f, 90.0f, 1.0);

				OPRENDERER_ACTIVE->SetViewport(0, 0, 32, 32);

				OPRENDERER_ACTIVE->SetDepthFunc(OPdepthFunction::LESS);
				ConvoluteEffect.Bind();
				ConvoluteEffect.Set("uEnvironmentMap", &textureCube, 1);
				ConvoluteEffect.Set("uProj", &views[0].proj);
				ConvoluteEffect.Set("uView", &views[side].view);


				//OPRENDERER_ACTIVE->SetDepthFunc(OPdepthFunction::LEQUAL);
				//SkyboxEffect.Bind();
				//SkyboxEffect.Set("uEnvironmentMap", &textureCube, 0);
				//SkyboxEffect.Set("uProj", &views[0].proj);
				//SkyboxEffect.Set("uView", &views[side].view);

				cubeMesh->Bind();
				OPrenderDrawBufferIndexed(0);

			}
			else {
				OPRENDERER_ACTIVE->SetDepthFunc(OPdepthFunction::LEQUAL);
				SkyboxEffect.Bind();
				SkyboxEffect.Set("uEnvironmentMap", &convoluteCube, 0);
				SkyboxEffect.Set("uProj", &camera.Camera.proj);
				SkyboxEffect.Set("uView", &camera.Camera.view);
				cubeMesh->Bind();
				OPrenderDrawBufferIndexed(0);
			}
		}
		else if (pbrState == PBRStates::PBR_HDRFILTER) {

			OPRENDERER_ACTIVE->SetDepthFunc(OPdepthFunction::LEQUAL);
			SkyboxEffect.Bind();
			//SkyboxEffect.Set("uEnvironmentMap", &testTextureCube, 0);
			SkyboxEffect.Set("uEnvironmentMap", &hdrRoughnessFilteredCube, 0);
			SkyboxEffect.Set("uProj", &camera.Camera.proj);
			SkyboxEffect.Set("uView", &camera.Camera.view);
			cubeMesh->Bind();
			OPrenderDrawBufferIndexed(0);

			OPRENDERER_ACTIVE->SetDepthFunc(OPdepthFunction::LESS);

			// OPtexture2DRender(BRDFTex2D);

			{
				BRDFEffect.Bind();
				//BRDFEffect.Set("uTexture", &brdfTexture, 0);

				quad->Bind();
				OPrenderDrawBufferIndexed(0);
			}



		}
		else if (pbrState == PBRStates::PBR_EFFECT) {

			OPRENDERER_ACTIVE->SetDepthFunc(OPdepthFunction::LEQUAL);

			if (PBREXAMPLE_MODELCOUNT > 0) {
				pbrModels[0].world.RotZ(0.01f);
			}

			{
				TIMED_BLOCK;

				for (ui32 i = 0; i < PBREXAMPLE_MODELCOUNT; i++) {
					pbrModels[i].material->Bind();
					pbrModels[i].material->SetWorld(&pbrModels[i].world);
					pbrModels[i].material->SetCamera(&camera.Camera);
					pbrModels[i].model->Bind();
					OPrenderDrawBufferIndexed(0);
				}

			}

			{
				TIMED_BLOCK;
				SkyboxEffect.Bind();
				SkyboxEffect.Set("uEnvironmentMap", &textureCube, 0);
				SkyboxEffect.Set("uProj", &camera.Camera.proj);
				SkyboxEffect.Set("uView", &camera.Camera.view);
				cubeMesh->Bind();
				OPrenderDrawBufferIndexed(0);
			}

		}
		OPVISUALDEBUGINFO.Render(delta);
		
		{
			TIMED_BLOCK;
			OPrenderPresent();
		}
	}

	OPint Exit(OPgameState* next) {
		scene.Destroy();
		renderer.Destroy();
		return 0;
	}
};



OPint GS_EXAMPLE_PBR_AVAILABLE = 1;
PBRExample _GS_EXAMPLE_PBR;
OPgameState* GS_EXAMPLE_PBR = &_GS_EXAMPLE_PBR;

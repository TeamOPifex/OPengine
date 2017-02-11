#include <include/Rendering/Font/OPfontManager.h>
#include <include/Rendering/Font/OPfont.h>
#include "./Human/include/Rendering/Font/OPfontManager.h"
#include "./Human/include/Rendering/OPrender.h"
#include "./Human/include/Systems/OPfontSystem.h"
#include "./Human/include/Rendering/OPmeshPacked.h"
#include "./Data/include/OPcman.h"
#include "./Core/include/Assert.h"

OPfontManager* OPFONTMANAGER_ACTIVE = NULL;
OPeffect* OPFONTMANAGER_EFFECT_ACTIVE = NULL;
OPeffect* OPFONTMANAGER_EFFECT2D_ACTIVE = NULL;

OPfontManager* OPfontManager::Init(OPfont* font) {
	OPfontSystemLoadEffects();

	_font = font;
	_color = OPvec4Create(0.0f, 0.0f, 0.0f, 1.0f);
	_align = OPfontAlign::LEFT;

	scale = 1.0f;
	currNodes = OPvector::Create(sizeof(OPfontTextNode));
	isBuilt = false;
	builtNodes = OPhashMap::Create(16);
	meshPacker.Init();
	proj = OPmat4Ortho(0, (OPfloat)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, (OPfloat)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, 0.0f, -1.0f, 1.0f);
	dummyMesh = _font->CreateUserText("", scale);

	return this;
}

OPfontManager* OPfontManager::Init(const OPchar* font, const OPchar** text, ui16 count) {
	OPfont* _font = (OPfont*)OPCMAN.LoadGet(font);
	Init(_font);

	this->scale = 1.0;
	this->_color = OPvec4Create(1,1,1,1);
	this->Bind();
	for (ui16 i = 0; i < count; i++) {
		this->AddText(text[i]);
	}
	this->Build();
	OPFONTMANAGER_ACTIVE = NULL;

	return this;
}

void OPfontManager::Destroy() {
	Unbind();
	currNodes->Destroy();
	OPfree(currNodes);

	OPhashMapBucket* bucket;
	OPuint i, j, n, m;
	OPhashMapPair *pair;

	n = builtNodes->count;
	bucket = builtNodes->buckets;
	i = 0;
	while (i < n) {
		m = bucket->count;
		pair = bucket->pairs;
		j = 0;
		while (j < m) {
			// mark asset for removal
			OPfontBuiltTextNode* node = (OPfontBuiltTextNode*)pair->value;
			node->packedMesh->Destroy();
			OPfree(node);
			pair++;
			j++;
		}
		bucket++;
		i++;
	}

	builtNodes->Destroy();
	OPfree(builtNodes);

	meshPacker.Destroy();
}

void OPfontManager::SetColor(f32 r, f32 g, f32 b, f32 a) {
	_color.x = r;
	_color.y = g;
	_color.z = b;
	_color.w = a;
}

void OPfontManager::AddText(const OPchar* text) {
	meshPacker.Bind();
	OPfontBuiltTextNode* node = OPNEW(OPfontBuiltTextNode());
	*node = _font->CreatePackedText(text, scale);
	builtNodes->Put(text, node);
}

void OPfontManager::Build() {
	dummyMesh.mesh.Bind();
	meshPacker.Bind();
	meshPacker.Build();
	isBuilt = true;
}

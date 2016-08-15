#pragma once

struct OPfontManager;
typedef struct OPfontManager OPfontManager;

#include "./Human/include/Rendering/Font/OPfont.h"
#include "./Human/include/Rendering/Font/OPfontTextNode.h"
#include "./Human/include/Rendering/Enums/OPfontAlign.h"

#include "./Human/include/Rendering/OPeffect.h"
#include "./Math/include/Vectors.h"
#include "./Data/include/OPhashMap.h"

extern OPfontManager* OPFONTMANAGER_ACTIVE;
extern OPeffect* OPFONTMANAGER_EFFECT_ACTIVE;
extern OPeffect* OPFONTMANAGER_EFFECT2D_ACTIVE;

struct OPfontManager {
	OPfontUserTextNode dummyMesh;
	OPfont* _font;
	OPvec4 _color;
	OPfontAlign _align;

	OPvector* currNodes;
	bool isBuilt;
	OPhashMap* builtNodes;
	OPmeshPacker meshPacker;
	OPint pixelated;
	OPmat4 proj;
	OPfloat scale;

	OPfontManager() {}
	OPfontManager(OPfont* font) { Init(font); }
	OPfontManager(const OPchar* font, const OPchar** text, ui16 count) { Init(font, text, count); }

	OPfontManager* Init(OPfont* font);
	OPfontManager* Init(const OPchar* font, const OPchar** text, ui16 count);

	void AddText(const OPchar* text);
	void Build();
	void SetColor(f32 r, f32 g, f32 b, f32 a);
	void Destroy();

	inline void Bind() {
		OPFONTMANAGER_ACTIVE = this;
	}

	inline void Unbind() {
		OPFONTMANAGER_ACTIVE = NULL;
	}

	inline void SetColor(OPvec3 color) {
		SetColor(color.x, color.y, color.z, 1.0);
	}

	inline void SetColor(OPvec4 color) {
		SetColor(color.x, color.y, color.z, color.w);
	}

	inline void SetColor(f32 r, f32 g, f32 b) {
		SetColor(r, g, b, 1.0);
	}
	
	inline void SetAlign(OPfontAlign align) {
		_align = align;
	}

	inline static OPfontManager* Create(OPfont* font) {
		return OPNEW(OPfontManager(font));
	}

	inline static OPfontManager* Create(const OPchar* font, const OPchar** text, ui16 count) {
		return OPNEW(OPfontManager(font, text, count));
	}
};

inline void OPfontEffectBind(OPeffect* effect) {
	OPFONTMANAGER_EFFECT_ACTIVE = effect;
}
#ifndef OPENGINE_HUMAN_RENDERER_FONT_RENDERTEXT
#define OPENGINE_HUMAN_RENDERER_FONT_RENDERTEXT

#include "./Human/include/Rendering/Font/OPfontManager.h"
#include "./Core/include/Assert.h"

void OPfontRender(const OPchar* text, OPvec4 color, OPvec2 pos, OPfontAlign align);
void OPfontRender(const OPchar* text, OPvec4 color, OPmat4* world, OPfontAlign align);

inline void OPfontRender(const OPchar* text, OPvec4 color, OPmat4* world) {
	OPfontRender(text, color, world, OPFONTMANAGER_ACTIVE->_align);
}
inline void OPfontRender(const OPchar* text, OPmat4* world) {
	OPfontRender(text, OPFONTMANAGER_ACTIVE->_color, world, OPFONTMANAGER_ACTIVE->_align);
}
inline void OPfontRender(const OPchar* text, f32 r, f32 g, f32 b, f32 a, f32 x, f32 y, OPfontAlign align) {
	OPfontRender(text, OPvec4Create(r, g, b, a), OPvec2Create(x, y), align);
}
inline void OPfontRender(const OPchar* text, f32 x, f32 y) {
	ASSERT(OPFONTMANAGER_ACTIVE != NULL, "A Font Manager has not been bound yet");
	OPfontRender(text, OPFONTMANAGER_ACTIVE->_color, OPvec2Create(x, y), OPFONTMANAGER_ACTIVE->_align);
}
inline void OPfontRender(const OPchar* text, f32 x, f32 y, OPfontAlign align) {
	ASSERT(OPFONTMANAGER_ACTIVE != NULL, "A Font Manager has not been bound yet");
	OPfontRender(text, OPFONTMANAGER_ACTIVE->_color, OPvec2Create(x, y), align);
}
inline void OPfontRender(const OPchar* text, f32 r, f32 g, f32 b, f32 x, f32 y) {
	ASSERT(OPFONTMANAGER_ACTIVE != NULL, "A Font Manager has not been bound yet");
	OPfontRender(text, OPvec4Create(r, g, b, 1.0f), OPvec2Create(x, y), OPFONTMANAGER_ACTIVE->_align);
}
inline void OPfontRender(const OPchar* text, f32 r, f32 g, f32 b, f32 a, f32 x, f32 y) {
	ASSERT(OPFONTMANAGER_ACTIVE != NULL, "A Font Manager has not been bound yet");
	OPfontRender(text, OPvec4Create(r,g,b,a), OPvec2Create(x,y), OPFONTMANAGER_ACTIVE->_align);
}
inline void OPfontRender(const OPchar* text, OPvec2 pos) {
	ASSERT(OPFONTMANAGER_ACTIVE != NULL, "A Font Manager has not been bound yet");
	OPfontRender(text, OPFONTMANAGER_ACTIVE->_color, pos, OPFONTMANAGER_ACTIVE->_align);
}
inline void OPfontRender(const OPchar* text, OPvec2 pos, OPfontAlign align) {
	ASSERT(OPFONTMANAGER_ACTIVE != NULL, "A Font Manager has not been bound yet");
	OPfontRender(text, OPFONTMANAGER_ACTIVE->_color, pos, align);
}
inline void OPfontRender(const OPchar* text, OPvec3 color, OPvec2 pos) {
	OPfontRender(text, OPvec4Create(color, 1.0f), pos, OPFONTMANAGER_ACTIVE->_align);
}
inline void OPfontRender(const OPchar* text, OPvec3 color, OPvec2 pos, OPfontAlign align) {
	OPfontRender(text, OPvec4Create(color, 1.0f), pos, align);
}
inline void OPfontRender(const OPchar* text, OPvec4 color, OPvec2 pos) {
	ASSERT(OPFONTMANAGER_ACTIVE != NULL, "A Font Manager has not been bound yet");
	OPfontRender(text, color, pos, OPFONTMANAGER_ACTIVE->_align);
}
inline void OPfontRender(const OPchar* text, f32 r, f32 g, f32 b, f32 a, OPmat4* world) {
	OPfontRender(text, OPvec4Create(r, g, b, a), world);
}
inline void OPfontRender(const OPchar* text, f32 r, f32 g, f32 b, OPmat4* world) {
	OPfontRender(text, r, g, b, 1.0, world);
}

#endif
#ifndef OPENGINE_HUMAN_RENDERER_FONT_RENDERTEXT
#define OPENGINE_HUMAN_RENDERER_FONT_RENDERTEXT

#include "./Human/include/Rendering/Font/OPfontManager.h"
#include "./Core/include/Assert.h"

void OPrenderText(const OPchar* text, OPvec4 color, OPvec2 pos, OPfontAlign align);
void OPrenderText(const OPchar* text, OPvec4 color, OPmat4* world);

inline void OPrenderText(const OPchar* text, OPmat4* world) {
	OPrenderText(text, OPRENDER_CURR_FONT_MANAGER->_color, world);
}
inline void OPrenderText(const OPchar* text, f32 r, f32 g, f32 b, f32 a, f32 x, f32 y, OPfontAlign align) {
	OPrenderText(text, OPvec4create(r, g, b, a), OPvec2create(x, y), align);
}
inline void OPrenderText(const OPchar* text, f32 x, f32 y) {
	ASSERT(OPRENDER_CURR_FONT_MANAGER != NULL, "A Font Manager has not been bound yet");
	OPrenderText(text, OPRENDER_CURR_FONT_MANAGER->_color, OPvec2create(x, y), OPRENDER_CURR_FONT_MANAGER->_align);
}
inline void OPrenderText(const OPchar* text, f32 x, f32 y, OPfontAlign align) {
	ASSERT(OPRENDER_CURR_FONT_MANAGER != NULL, "A Font Manager has not been bound yet");
	OPrenderText(text, OPRENDER_CURR_FONT_MANAGER->_color, OPvec2create(x, y), align);
}
inline void OPrenderText(const OPchar* text, f32 r, f32 g, f32 b, f32 x, f32 y) {
	ASSERT(OPRENDER_CURR_FONT_MANAGER != NULL, "A Font Manager has not been bound yet");
	OPrenderText(text, OPvec4create(r, g, b, 1.0f), OPvec2create(x, y), OPRENDER_CURR_FONT_MANAGER->_align);
}
inline void OPrenderText(const OPchar* text, f32 r, f32 g, f32 b, f32 a, f32 x, f32 y) {
	ASSERT(OPRENDER_CURR_FONT_MANAGER != NULL, "A Font Manager has not been bound yet");
	OPrenderText(text, OPvec4create(r,g,b,a), OPvec2create(x,y), OPRENDER_CURR_FONT_MANAGER->_align);
}
inline void OPrenderText(const OPchar* text, OPvec2 pos) {
	ASSERT(OPRENDER_CURR_FONT_MANAGER != NULL, "A Font Manager has not been bound yet");
	OPrenderText(text, OPRENDER_CURR_FONT_MANAGER->_color, pos, OPRENDER_CURR_FONT_MANAGER->_align);
}
inline void OPrenderText(const OPchar* text, OPvec2 pos, OPfontAlign align) {
	ASSERT(OPRENDER_CURR_FONT_MANAGER != NULL, "A Font Manager has not been bound yet");
	OPrenderText(text, OPRENDER_CURR_FONT_MANAGER->_color, pos, align);
}
inline void OPrenderText(const OPchar* text, OPvec3 color, OPvec2 pos) {
	OPrenderText(text, OPvec4createFromVec3(color, 1.0f), pos, OPRENDER_CURR_FONT_MANAGER->_align);
}
inline void OPrenderText(const OPchar* text, OPvec3 color, OPvec2 pos, OPfontAlign align) {
	OPrenderText(text, OPvec4createFromVec3(color, 1.0f), pos, align);
}
inline void OPrenderText(const OPchar* text, OPvec4 color, OPvec2 pos) {
	ASSERT(OPRENDER_CURR_FONT_MANAGER != NULL, "A Font Manager has not been bound yet");
	OPrenderText(text, color, pos, OPRENDER_CURR_FONT_MANAGER->_align);
}
inline void OPrenderText(const OPchar* text, f32 r, f32 g, f32 b, f32 a, OPmat4* world) {
	OPrenderText(text, OPvec4create(r, g, b, a), world);
}
inline void OPrenderText(const OPchar* text, f32 r, f32 g, f32 b, OPmat4* world) {
	OPrenderText(text, r, g, b, 1.0, world);
}

#endif
#include "./Human/include/Input/OPkeyboard.h"
#include "./Human/include/Input/OPinputRecord.h"
#include "./Human/include/Input/Enums/OPkeyboardKeys.h"
#include "./Human/include/Rendering/OPrender.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/Assert.h"

OPkeyboard OPKEYBOARD;

#if !defined(OPIFEX_ANDROID) && !defined(OPIFEX_IOS)

void (*OPKEYBOARD_STREAM)(OPchar);

void OPkeyboard::Update(OPtimer* timer) {
	ASSERT(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE != NULL, "There must be an active window");

	OPmemcpy(&prevKeys, &keys, (ui32)OPkeyboardKey::_MAX * sizeof(bool));
	for(ui32 i = 0; i < (ui32)OPkeyboardKey::_MAX; i++) {
		keys[i] = OPRENDERER_ACTIVE->Window.GetKeyboardState(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE, (OPkeyboardKey)i);
	}
}

inline void OPkeyboard::UpdatePost(OPtimer* timer) {
	OPinputRecordUpdate(timer);
}

bool OPkeyboard::AnyInputIsDown() {
	for (ui32 i = 0; i < (ui32)OPkeyboardKey::_MAX; i++) {
		if (keys[i]) return true;
	}
	return false;
}

#else

void (*OPKEYBOARD_STREAM)(OPchar);

void OPkeyboard::Update(OPtimer* timer) {

}

void OPkeyboard::UpdatePost(OPtimer* timer) {

}


#endif

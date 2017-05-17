#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./OPengine.h"


void keystream(OPchar c);
class IMGUIExample : public OPgameState {
public:
	OPeffect effect;
	OPchar buffer[100];
	OPint textTimer;
	OPint textActive;
	OPvec3 clear;
	OPint checked;
	OPfontManager* FontManager;
	OPimgui* imgui;


	void Init(OPgameState* last) {

		OPKEYBOARD_STREAM = keystream;

		const OPchar* data = "Testing";
		for (ui32 i = 0; i < strlen(data); i++) {
			buffer[i] = data[i];
		}
		textTimer = 0;
		textActive = 0;
		clear = OPvec3Create(0, 0, 0);

		FontManager = OPfontManager::Create("Ubuntu.opf", NULL, 0);
		FontManager->scale = 0.75;


		effect.Init("imgui.vert", "imgui.frag");
		imgui = OPimguiCreate(&effect, FontManager);
		imgui->primaryColor = OPvec4(1, 0, 0, 1);
		imgui->secondaryColor = OPvec4(1, 1, 1, 1);
		imgui->hoverColor = OPvec4(0.9f, 0.9f, 0.9f, 1);
		imgui->padding = OPvec4(10, 10, 10, 10);

		OPimguiBind(imgui);
	}

	OPint Update(OPtimer* time) {


		if (OPKEYBOARD.WasPressed(OPkeyboardKey::BACKSPACE) && textActive && strlen(buffer) > 0) {
			OPlog("Buff %s", buffer);
			buffer[strlen(buffer) - 1] = '\0';
			OPlog("Buff %s", buffer);
		}

		textTimer += time->Elapsed;


		return false;

	}

	void Render(OPfloat delta) {

		OPrenderClear(clear);

		OPimguiBegin();

		if (OPimguiCheckbox(checked, OPvec2(570, 20), OPvec2(20, 20))) { //, OPvec4Create(0.9, 0.9, 0.9, 1), OPvec4Create(0.1,0.1,0.1,1));
			checked = !checked;
		}
		//OPimguiLabel(OPvec2(50, 50), "TEST");
		//OPimguiRadio(OPvec2(5,5), OPvec2(20, 20), OPvec4Create(0.9, 0.9, 0.9, 1), OPvec4Create(0.1,0.1,0.1, 1));
		//OPimguiRadio(exampleSelector.imgui, OPvec2(300,150), OPvec2(10, 10), OPvec4Create(0.7, 0.7, 0.7, 1));

		if (OPimguiButton(OPvec2(440, 20), "TOGGLE")) {
			clear.z = !clear.z;
		}

		//OPlog(" Active %d", exampleSelector.textActive);
		OPint state = OPimguiTextbox(OPvec2(20, 20), buffer, "Your Name", textActive, textTimer % 500 > 250);
		if (state == 1) textActive = true;
		else if (state == 2) textActive = false;

		OPimguiEnd();

		OPrenderPresent();
	}

	OPint Exit(OPgameState* next) {
		if (textActive) return 1;

		return 0;
	}
};



IMGUIExample _GS_EXAMPLE_IMGUI;
void keystream(OPchar c) {
	if (!_GS_EXAMPLE_IMGUI.textActive) return;
	OPint p = strlen(_GS_EXAMPLE_IMGUI.buffer);
	_GS_EXAMPLE_IMGUI.buffer[p] = c;
	_GS_EXAMPLE_IMGUI.buffer[p + 1] = '\0';
	OPlog("MyCode %c", c);
}

OPint GS_EXAMPLE_IMGUI_AVAILABLE = 1;
// This is the Game State for this IMGUIExample
// Each entry is a function pointer for Initialize, Update, Destroy
OPgameState* GS_EXAMPLE_IMGUI = &_GS_EXAMPLE_IMGUI;
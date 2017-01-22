#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./OPengine.h"

#ifdef ADDON_imgui
#include "OPimgui.h"
#endif
class OcornutIMGUIExample : public OPgameState {

	void Init(OPgameState* last) {
#ifdef ADDON_imgui
		OPimguiInit(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE, true);
#endif
	}

#ifdef ADDON_imgui
	bool show_another_window = false;
	bool show_test_window = false;
	ImVec4 clear_color = ImColor(114, 144, 154);
#endif
	OPint Update(OPtimer* time) {

		return false;
	}

	void Render(OPfloat delta) {

#ifdef ADDON_imgui
		OPlog("Frame Started");
		OPimguiNewFrame();
		{
			static float f = 0.0f;
			ImGui::Text("Hello, world!");
			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
			ImGui::ColorEdit3("clear color", (float*)&clear_color);
			if (ImGui::Button("Test Window")) show_test_window ^= 1;
			if (ImGui::Button("Another Window")) show_another_window ^= 1;
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}

		// 2. Show another simple window, this time using an explicit Begin/End pair
		if (show_another_window)
		{
			ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
			ImGui::Begin("Another Window", &show_another_window);
			ImGui::Text("Hello");
			ImGui::End();
		}

		// 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
		if (show_test_window)
		{
			ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
			ImGui::ShowTestWindow(&show_test_window);
		}

#endif

		OPrenderClear(0, 0, 0);
#ifdef ADDON_imgui
		ImGui::Render();
#endif
		OPrenderPresent();
	}

	OPint Exit(OPgameState* next) {
		return 0;
	}
};


#ifdef ADDON_imgui
OPint GS_EXAMPLE_OCORNUT_IMGUI_AVAILABLE = 1;
#else
OPint GS_EXAMPLE_OCORNUT_IMGUI_AVAILABLE = 0;
#endif
// This is the Game State for this OcornutIMGUIExample
// Each entry is a function pointer for Initialize, Update, Destroy
OcornutIMGUIExample _GS_EXAMPLE_OCORNUT_IMGUI;
OPgameState* GS_EXAMPLE_OCORNUT_IMGUI = &_GS_EXAMPLE_OCORNUT_IMGUI;
#include "./Pipeline/include/OPvisualDebugInfo.h"
#include "./Human/include/Input/OPkeyboard.h"
#include "./Data/include/OPhashMap.h"
#include "./Data/include/OPstring.h"

#ifdef ADDON_imgui

#include "OPimgui.h"
static float arr[1000];
static int ind = 0;
static float arr2[250];
static int ind2 = 0;
static float arr3[250];
static int ind3 = 0;
static float arr4[100];
static int ind4 = 0;
static bool openDebugInfo = false;
static bool openDebugInfoMemory = false;
static bool PAUSED = false;

struct _memoryBlock {
	const OPchar* _func;
	ui64 _bytes;
	const OPchar* _file;
	ui64 _line;
};

static _memoryBlock _allocationsTracker[1000];
static ui64 _allocationsTrackerInd = 0;

static const OPchar* _allocationFunctions[1000];
static OPuint _allocationFunctionTotals[1000];
static OPuint _allocationFunctionCalls[1000];
static ui64 _allocationFunctionsInd = 0;

void sort()
{
	int i, j, k;
	//printf("%d", count);

	OPuint temp, temp3;
	const OPchar* temp2;
	do {
		j = 0;
		for (i = 0; i <= _allocationFunctionsInd; i++) {
			if (_allocationFunctionTotals[i] < _allocationFunctionTotals[i + 1]) {//this was numbers[k], which was an error
				j = 1;
				temp = _allocationFunctionTotals[i];
				temp2 = _allocationFunctions[i];
				temp3 = _allocationFunctionCalls[i];
				_allocationFunctionTotals[i] = _allocationFunctionTotals[i + 1];
				_allocationFunctions[i] = _allocationFunctions[i + 1];
				_allocationFunctionCalls[i] = _allocationFunctionCalls[i + 1];
				_allocationFunctionTotals[i + 1] = temp;
				_allocationFunctions[i + 1] = temp2;
				_allocationFunctionCalls[i + 1] = temp3;
			}
		}
	} while (j == 1);
}

void RecalculateFunctionMemory() {

	_allocationFunctionsInd = 0;
	bool found = false;
	for (ui32 i = 0; i < _allocationsTrackerInd; i++) {
		found = false;
		for (ui32 j = 0; j < _allocationFunctionsInd; j++) {
			if (OPstringEquals(_allocationFunctions[j], _allocationsTracker[i]._func)) {
				found = true;
				_allocationFunctionTotals[j] += _allocationsTracker[i]._bytes;
				_allocationFunctionCalls[j]++;
				break;
			}
		}
		if (!found) {
			_allocationFunctions[_allocationFunctionsInd] = _allocationsTracker[i]._func;
			_allocationFunctionTotals[_allocationFunctionsInd] = _allocationsTracker[i]._bytes;
			_allocationFunctionCalls[_allocationFunctionsInd] = 1;
			_allocationFunctionsInd++;
			_allocationFunctionsInd = _allocationFunctionsInd % 1000;
		}
	}

	// Sort high to low
	sort();
}

void _memoryTrackerFunc(const OPchar* func, ui64 bytes, const OPchar* file, ui64 line ) {
	_allocationsTracker[_allocationsTrackerInd]._func = func;
	_allocationsTracker[_allocationsTrackerInd]._bytes = bytes;
	_allocationsTracker[_allocationsTrackerInd]._file = file;
	_allocationsTracker[_allocationsTrackerInd]._line = line;
	_allocationsTrackerInd++;
	_allocationsTrackerInd = _allocationsTrackerInd % 1000;
	RecalculateFunctionMemory();
}

#endif

OPvisualDebugInfo OPVISUALDEBUGINFO;


void OPvisualDebugInfo::PreInitSetup() {
#ifdef ADDON_imgui
	#ifdef _DEBUG
		OPALLOCATIONTRACKER = _memoryTrackerFunc;
	#endif
#endif
}

void OPvisualDebugInfo::Init() {
#ifdef ADDON_imgui
    PreInitSetup();
	OPimguiInit(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE, true);
	arr[0] = 0;
	arr2[0] = 0;
	arr3[0] = 0;
#endif
}

void OPvisualDebugInfo::Update(OPtimer* timer) {
#ifdef ADDON_imgui
	if (OPKEYBOARD.WasPressed(OPkeyboardKey::F1)) {
		openDebugInfo = !openDebugInfo;
	}
	if (OPKEYBOARD.WasPressed(OPkeyboardKey::F2)) {
		openDebugInfoMemory = !openDebugInfoMemory;
	}
	if (OPKEYBOARD.WasPressed(OPkeyboardKey::F3)) {
		PAUSED = !PAUSED;
	}

	arr2[ind2] += timer->Elapsed;

	ind3++;
	ind3 = ind3 % 250;
	arr3[ind3] = 0;
#endif
}

void OPvisualDebugInfo::Begin() {
#ifdef ADDON_imgui
	OPimguiNewFrame();
#endif
}

void OPvisualDebugInfo::DrawWindows(OPfloat delta) {

#ifdef ADDON_imgui

	// Add Debug Info Here
	if (!openDebugInfo) return;

	f32 bytesInKB = 1024;
	f32 bytesInMB = bytesInKB * 1024;

	ImGui::SetNextWindowPos(ImVec2(10, 10));
	if (!ImGui::Begin("Example: Fixed Overlay", &openDebugInfo, ImVec2(0, 0), 0.3f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
	{
		ImGui::End();
		return;
	}

	ImGui::Text("OPengine Debug (F1)");
	ImGui::Text("Mouse Position: (%.1f,%.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Separator();

#ifdef _DEBUG
	if (OPallocationBytes > bytesInMB) {
		ImGui::Text("Bytes Allocated: %.2f MB", OPallocationBytes / (f32)bytesInMB);
	}
	else if (OPallocationBytes > bytesInKB) {
		ImGui::Text("Bytes Allocated: %.2f KB", OPallocationBytes / (f32)bytesInKB);
	}
	else {
		ImGui::Text("Bytes Allocated: %d bytes", OPallocationBytes);
	}

	OPuint bytesCurrently = OPallocationBytes - OPdeallocationBytes;
	if (bytesCurrently > bytesInMB) {
		ImGui::Text("Bytes Currently Allocated: %.2f MB", bytesCurrently / (f32)bytesInMB);
	}
	else if (bytesCurrently > bytesInKB) {
		ImGui::Text("Bytes Currently Allocated: %.2f KB", bytesCurrently / (f32)bytesInKB);
	}
	else {
		ImGui::Text("Bytes Currently Allocated: %d bytes", bytesCurrently);
	}

	if (OPallocationBytesRequested > bytesInMB) {
		ImGui::Text("Bytes Requested: %.2f MB", OPallocationBytesRequested / (f32)bytesInMB);
	}
	else if (OPallocationBytesRequested > bytesInKB) {
		ImGui::Text("Bytes Requested: %.2f KB", OPallocationBytesRequested / (f32)bytesInKB);
	}
	else {
		ImGui::Text("Bytes Requested: %d bytes", OPallocationBytesRequested);
	}

	ImGui::Text("Total Allocations: %d", OPallocations);
	ImGui::Text("Total Deallocations: %d", OPdeallocations);
#endif
	ImGui::Separator();


	if (!PAUSED) {

		arr[ind] = ImGui::GetIO().Framerate;
		ind++;
		ind = ind % 1000;

		ind2++;
		ind2 = ind2 % 250;
		arr2[ind2] = 0;

		arr3[ind3]++;

		arr4[ind4] = delta;
		ind4++;
		ind4 = ind4 % 100;
	}


	ImGui::PlotHistogram("FPS", arr, 1000, ind, "", 0, 200, ImVec2(0, 80));
	ImGui::PlotHistogram("Elapsed", arr2, 250, ind2, "", 0, 32, ImVec2(0, 80));
	ImGui::PlotHistogram("Frames Per Update", arr3, 250, ind3, "", 0, 3, ImVec2(0, 80));
	ImGui::PlotHistogram("Delta", arr4, 100, ind4, "", 0, 1, ImVec2(0, 80));


	ImGui::End();

	if (openDebugInfoMemory) {
		ImGui::Begin("Memory");
		{
			ImGui::Columns(4, "mycolumns"); // 2-ways, with border
			ImGui::Separator();
			ImGui::Text("Function"); ImGui::NextColumn();
			ImGui::Text("Bytes"); ImGui::NextColumn();
			ImGui::Text("File"); ImGui::NextColumn();
			ImGui::Text("Line"); ImGui::NextColumn();
			ImGui::Separator();
			static int selected = -1;
			for (int i = 0; i < _allocationsTrackerInd; i++)
			{
				if (_allocationsTracker[i]._func != NULL) {
					ImGui::Text(_allocationsTracker[i]._func);
				}
				ImGui::NextColumn();

				char label[32];


				if (_allocationsTracker[i]._bytes > bytesInMB) {
					sprintf(label, "%.2f MB", _allocationsTracker[i]._bytes / bytesInMB);
				}
				else if (_allocationsTracker[i]._bytes > 1024) {
					sprintf(label, "%.2f KB", _allocationsTracker[i]._bytes / bytesInKB);
				}
				else {
					sprintf(label, "%d bytes", _allocationsTracker[i]._bytes);
				}

				if (ImGui::Selectable(label, selected == i, ImGuiSelectableFlags_SpanAllColumns))
					selected = i;

				ImGui::NextColumn();
				if (_allocationsTracker[i]._file != NULL) {
					ImGui::Text(_allocationsTracker[i]._file);
				}
				ImGui::NextColumn();
				sprintf(label, "%d", _allocationsTracker[i]._line);
				ImGui::Text(label);
				ImGui::NextColumn();
			}
			ImGui::Columns(1);
			ImGui::Separator();
		}
		ImGui::End();

		ImGui::Begin("Memory by Functions");
		{
			ImGui::Columns(3, "mycolumnsFuncs"); // 2-ways, with border
			ImGui::Separator();
			ImGui::Text("Function"); ImGui::NextColumn();
			ImGui::Text("Bytes"); ImGui::NextColumn();
			ImGui::Text("Calls"); ImGui::NextColumn();
			ImGui::Separator();
			static int selected = -1;

			void* data;
			ui64 bytesToShow;
			for (int i = 0; i < _allocationFunctionsInd; i++)
			{
				if (_allocationFunctions[i] == NULL) continue;

				bytesToShow = _allocationFunctionTotals[i];

				ImGui::Text(_allocationFunctions[i]);
				ImGui::NextColumn();

				char label[32];


				if (bytesToShow > bytesInMB) {
					sprintf(label, "%.2f MB", bytesToShow / bytesInMB);
				}
				else if (bytesToShow > 1024) {
					sprintf(label, "%.2f KB", bytesToShow / bytesInKB);
				}
				else {
					sprintf(label, "%d bytes", bytesToShow);
				}

				ImGui::Text(label);
				ImGui::NextColumn();

				sprintf(label, "%d", _allocationFunctionCalls[i]);
				ImGui::Text(label);
				ImGui::NextColumn();
			}
			ImGui::Columns(1);
			ImGui::Separator();
		}
		ImGui::End();
	}

	//ImGui::ShowTestWindow();
#endif
}

void OPvisualDebugInfo::Render(OPfloat delta) {
#ifdef ADDON_imgui
	Begin();
	DrawWindows(delta);
	ImGui::Render();
#endif
}

void OPvisualDebugInfo::Destroy() {
#ifdef ADDON_imgui
	//OPimguiDestroy()
#endif
}

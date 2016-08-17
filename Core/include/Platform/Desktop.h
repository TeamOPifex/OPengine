#pragma once

#include "./Core/include/OPtypes.h"
#include "./Core/include/OPtimer.h"

#ifdef OPIFEX_DESKTOP

	#ifdef OPIFEX_WINDOWS
	extern HINSTANCE OP_HINSTANCE;
	#endif

	/**
	* Begins the game cycle.
	*	This function is responsible for several actions. It creates an
	*	OPtimer instance, and invokes the function pointer OPinitialize() to
	*	perform user defined initializations. Once initialized, the game loop
	*	is started. The timer instance is updated on each iteration and passed
	*	to the OPupdate function pointer. The game loop runs until OPend() is
	*	called. At which point the OPdestroy() function pointer is called and
	*	and clean up is performed.
	* @param argc Number of arguments passed through on start
	* @param args Each of the arguments passed into the program at start
	*/
	void OPstart(int argc, char** args);

	/**
	* Begins the game cycle but with a Stepped Update Interval.
	*	This function is responsible for several actions. It creates an
	*	OPtimer instance, and invokes the function pointer OPinitialize() to
	*	perform user defined initializations. Once initialized, the game loop
	*	is started. The timer instance is updated on each iteration and passed
	*	to the OPupdate function pointer. The game loop runs until OPend() is
	*	called. At which point the OPdestroy() function pointer is called and
	*	and clean up is performed.
	* @param argc Number of arguments passed through on start
	* @param args Each of the arguments passed into the program at start
	*/
	void OPstartStepped(int argc, char** args);

	#if defined(OPIFEX_OPTION_RELEASE) && defined(OPIFEX_WINDOWS)
		#ifdef _CONSOLE
			#define OP_MAIN_START int main(int argc, char** args) {
			#define OP_MAIN_RUN OPstart(argc, args);
			#define OP_MAIN_RUN_STEPPED OPstartStepped(argc, args);
		#else
			#define OP_MAIN_START int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) { OP_HINSTANCE = hInstance;
			#define OP_MAIN_RUN OPstart(0, NULL);
			#define OP_MAIN_RUN_STEPPED OPstartStepped(0, NULL);
		#endif
	#else
		#ifdef _CONSOLE
			#define OP_MAIN_START int main(int argc, char** args) {
			#define OP_MAIN_RUN OPstart(argc, args);
			#define OP_MAIN_RUN_STEPPED OPstartStepped(argc, args);
		#else
			#define OP_MAIN_START int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) { OP_HINSTANCE = hInstance;
			#define OP_MAIN_RUN OPstart(0, NULL);
			#define OP_MAIN_RUN_STEPPED OPstartStepped(0, NULL);
		#endif
	#endif

#endif
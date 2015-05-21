#include "./ExampleSelectorState.h"
#include "./Scripting/include/JavaScript/OPjavaScriptV8.h"
#include "./Scripting/include/OPloaderOPS.h"

#ifdef OPIFEX_OPTION_V8

typedef struct {
	OPscript* MyScript;
	OPjavaScriptV8Compiled Compiled;
} ScriptingExample;

ScriptingExample scriptingExample;

void ExampleScriptingEnter(OPgameState* last) {
	OPjavaScriptV8Init();
	
	scriptingExample.MyScript = (OPscript*)OPcmanLoadGet("main.js");
	OPjavaScriptV8Compile(&scriptingExample.Compiled, scriptingExample.MyScript);
	OPjavaScriptV8Run(&scriptingExample.Compiled);
}

OPint ExampleScriptingUpdate(OPtimer* time) {
	OPgameStateChange(&GS_EXAMPLE_SELECTOR);
	return false;
}

OPint ExampleScriptingExit(OPgameState* next) {
	return 0;
}

OPgameState GS_EXAMPLE_SCRIPTING = {
	ExampleScriptingEnter,
	ExampleScriptingUpdate,
	ExampleScriptingExit
};
#else

OPgameState GS_EXAMPLE_SCRIPTING;

#endif

#ifndef OPIFEX_OPTION_V8
OPint GS_EXAMPLE_SCRIPTING_AVAILABLE = 0;
#else
OPint GS_EXAMPLE_SCRIPTING_AVAILABLE = 1;
#endif
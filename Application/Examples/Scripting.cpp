#include "./ExampleSelectorState.h"
#include "./Scripting/include/V8/OPscriptV8.h"
#include "./Scripting/include/OPloaderOPS.h"

#ifdef OPIFEX_OPTION_V8

typedef struct {
	OPscript* MyScript;
	OPscriptV8Compiled Compiled;
} ScriptingExample;

ScriptingExample scriptingExample;

void ExampleScriptingEnter(OPgameState* last) {
	OPscriptV8Init();
	
	scriptingExample.MyScript = (OPscript*)OPcmanLoadGet("myscript.js");
	OPscriptV8Compile(&scriptingExample.Compiled, scriptingExample.MyScript, NULL);
	OPscriptV8Run(&scriptingExample.Compiled);
}

OPint ExampleScriptingUpdate(OPtimer* time) {
	//OPscriptCompileAndRun(scriptingExample.MyScript);
	// OPscriptValuePersistent values[3] = {
	// 	OPscriptGetValue("1"),
	// 	OPscriptGetValue(1.0),
	// 	OPscriptGetValue(1.0)
	// };
	// OPscriptValuePersistent val = OPscriptRunFunc(&scriptingExample.Compiled, "clearToBlack", 3, values);
	OPscriptV8Run(&scriptingExample.Compiled, "clearToBlack");

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
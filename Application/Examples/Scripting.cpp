#include "./ExampleSelectorState.h"
#include "./Scripting/include/OPscript.h"

void ExampleScriptingEnter(OPgameState* last);
OPint ExampleScriptingUpdate(OPtimer* time);
void ExampleScriptingExit(OPgameState* next);

OPgameState GS_EXAMPLE_SCRIPTING = {
	ExampleScriptingEnter,
	ExampleScriptingUpdate,
	ExampleScriptingExit
};

typedef struct {
	OPscript* MyScript;
	OPscriptCompiled Compiled;
} ScriptingExample;

ScriptingExample scriptingExample;

void ExampleScriptingEnter(OPgameState* last) {
	OPscriptInit();
	scriptingExample.MyScript = (OPscript*)OPcmanLoadGet("myscript.js");
	OPscriptCompile(&scriptingExample.Compiled, scriptingExample.MyScript);
	OPscriptRun(&scriptingExample.Compiled);
}

OPint ExampleScriptingUpdate(OPtimer* time) {
	//OPscriptCompileAndRun(scriptingExample.MyScript);
	OPscriptRunFunc(&scriptingExample.Compiled, "clearToBlack", 3, "0", "0", "1");

	return false;
}

void ExampleScriptingExit(OPgameState* next) {

}

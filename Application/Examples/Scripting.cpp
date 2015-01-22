#include "./ExampleSelectorState.h"
#include "./Scripting/include/V8/OPscriptV8.h"

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

void ExampleScriptingExit(OPgameState* next) {

}

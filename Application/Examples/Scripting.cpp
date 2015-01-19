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
	OPscriptCompileAndRun(&scriptingExample.Compiled, scriptingExample.MyScript, NULL);
}

OPint ExampleScriptingUpdate(OPtimer* time) {
	//OPscriptCompileAndRun(scriptingExample.MyScript);
	OPscriptValuePersistent values[3] = {
		OPscriptGetValue("1"),
		OPscriptGetValue(1.0),
		OPscriptGetValue(1.0)
	};
	OPscriptValuePersistent val = OPscriptRunFunc(&scriptingExample.Compiled, "clearToBlack", 3, values);
	OPscriptRunFunc(&scriptingExample.Compiled, "run", 1, &val);

	return false;
}

void ExampleScriptingExit(OPgameState* next) {

}

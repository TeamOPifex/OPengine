#include "./ExampleSelectorState.h"
#include "./Scripting/include/JavaScript/OPjavaScriptV8.h"
#include "./Scripting/include/OPloaderOPS.h"

#ifdef OPIFEX_OPTION_V8

typedef struct {
	OPscript* InitScript;
	OPscript* UpdateScript;
	OPscript* DestroyScript;
	OPjavaScriptV8Compiled InitCompiled;
	OPjavaScriptV8Compiled UpdateCompiled;
	OPjavaScriptV8Compiled DestroyCompiled;
} ScriptingExample;

ScriptingExample scriptingExample;

void ExampleScriptingEnter(OPgameState* last) {
	OPjavaScriptV8Init();

	scriptingExample.InitScript = (OPscript*)OPcmanLoadGet("init.js");
	scriptingExample.UpdateScript = (OPscript*)OPcmanLoadGet("update.js");
	scriptingExample.DestroyScript = (OPscript*)OPcmanLoadGet("exit.js");
	OPjavaScriptV8Compile(&scriptingExample.InitCompiled, scriptingExample.InitScript);
	OPjavaScriptV8Compile(&scriptingExample.UpdateCompiled, scriptingExample.UpdateScript);
	OPjavaScriptV8Compile(&scriptingExample.DestroyCompiled, scriptingExample.DestroyScript);

    OPjavaScriptV8Run(&scriptingExample.InitCompiled);
    OPjavaScriptV8Run(&scriptingExample.UpdateCompiled);
}

OPint ExampleScriptingUpdate(OPtimer* time) {
    SCOPE_AND_ISOLATE;

    OPjavaScriptPersistentValue args[1] = {
            JS_CREATE_PERSISTENT(JS_NEW_NUMBER(0.5))
    };
    OPjavaScriptPersistentValue result = OPjavaScriptV8Run(&scriptingExample.UpdateCompiled, "Update", 1, args);

	return JS_GET_PERSISTENT_NUMBER(result);
}

OPint ExampleScriptingExit(OPgameState* next) {
	OPjavaScriptV8Run(&scriptingExample.DestroyCompiled);
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
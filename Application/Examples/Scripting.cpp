#include "./ExampleSelectorState.h"
#include "./Scripting/include/JavaScript/OPjavaScriptV8.h"
#include "./Scripting/include/OPloaderOPS.h"

#ifdef OPIFEX_OPTION_V8

class ScriptingExample : public OPgameState {
	OPscript* InitScript;
	OPscript* UpdateScript;
	OPscript* DestroyScript;
	OPjavaScriptV8Compiled InitCompiled;
	OPjavaScriptV8Compiled UpdateCompiled;
	OPjavaScriptV8Compiled DestroyCompiled;

	void Init(OPgameState* last) {
		OPjavaScriptV8Init();

		InitCompiled = OPjavaScriptV8Compiled("init.js");
		UpdateCompiled = OPjavaScriptV8Compiled("update.js");
		DestroyCompiled = OPjavaScriptV8Compiled("exit.js");

		//InitScript = (OPscript*)OPCMAN.LoadGet("init.js");
		//UpdateScript = (OPscript*)OPCMAN.LoadGet("update.js");
		//DestroyScript = (OPscript*)OPCMAN.LoadGet("exit.js");
		//OPjavaScriptV8Compile(&InitCompiled, InitScript);
		//OPjavaScriptV8Compile(&UpdateCompiled, UpdateScript);
		//OPjavaScriptV8Compile(&DestroyCompiled, DestroyScript);

		OPjavaScriptV8Run(&InitCompiled);
		OPjavaScriptV8Run(&UpdateCompiled);
	}

	OPint Update(OPtimer* time) {

		SCOPE_AND_ISOLATE;

		// OPjavaScriptPersistentValue args[1] = {
		//         JS_CREATE_PERSISTENT(JS_NEW_NUMBER(0.5))
		// };
		// const OPchar* name = "Update";
		// OPjavaScriptPersistentValue result = OPjavaScriptV8Run(&UpdateCompiled, name, 1, args);
		//
		// return JS_GET_PERSISTENT_NUMBER(result);
		return 0;
	}

	void Render(OPfloat delta) {

	}

	OPint Exit(OPgameState* next) {
		OPjavaScriptV8Run(&DestroyCompiled);
		return 0;
	}
};


OPint GS_EXAMPLE_SCRIPTING_AVAILABLE = 1;
ScriptingExample _GS_EXAMPLE_SCRIPTING;
OPgameState* GS_EXAMPLE_SCRIPTING = &_GS_EXAMPLE_SCRIPTING;
#else

OPint GS_EXAMPLE_SCRIPTING_AVAILABLE = 0;
OPgameState* GS_EXAMPLE_SCRIPTING = NULL;

#endif
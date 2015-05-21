#ifndef OPENGINE_SCRIPTING_JAVASCRIPT_V8
#define OPENGINE_SCRIPTING_JAVASCRIPT_V8

#include "./Core/include/OPtypes.h"
#include "./Scripting/include/OPloaderOPS.h"

#ifdef OPIFEX_OPTION_V8

#include "./Scripting/include/JavaScript/OPjavaScriptHelper.h"

struct OPjavaScriptV8Compiled {
	OPscript* Source;
	OPjavaScriptPersistentScript Script;
	OPjavaScriptPersistentContext Context;
};

void OPjavaScriptV8Init();

OPint OPjavaScriptV8Compile(OPjavaScriptV8Compiled* compiled, OPscript* script, OPchar* dir);
void OPjavaScriptV8Update(OPjavaScriptV8Compiled* scriptCompiled);
OPjavaScriptPersistentValue OPjavaScriptV8Run(OPjavaScriptV8Compiled* scriptCompiled);
OPjavaScriptPersistentValue OPjavaScriptV8Run(OPjavaScriptV8Compiled* scriptCompiled, OPchar* name);
OPjavaScriptPersistentValue OPjavaScriptV8Run(OPjavaScriptV8Compiled* scriptCompiled, OPchar* name, OPuint count, OPjavaScriptPersistentValue* args);
inline OPint OPjavaScriptV8Compile(OPjavaScriptV8Compiled* compiled, OPscript* script) {
	return OPjavaScriptV8Compile(compiled, script, NULL);
}
//OPscriptV8PersistentValue OPjavaScriptV8Run(OPscriptV8Compiled* scriptCompiled, OPchar* name);
//OPscriptV8PersistentValue OPjavaScriptV8Run(OPscriptV8Compiled* scriptCompiled, OPchar* name, OPint count, OPscriptV8PersistentValue* args);


#ifdef _DEBUG
	#define OPJAVASCRIPTV8_UPDATE(compiled) OPjavaScriptV8Update(compiled);
#else
	#define OPJAVASCRIPTV8_UPDATE(compiled)
#endif


#endif

#endif
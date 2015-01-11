
#include "./Scripting/include/OPscriptV8.h"
#include "./Core/include/OPlog.h"

#ifdef OPIFEX_V8
Isolate* isolate = NULL;
OPint SCRIPT_INITIALIZED = 0;
#endif

void OPscriptInit() {
#ifdef OPIFEX_V8
	if (!SCRIPT_INITIALIZED) {
		v8::V8::InitializeICU();
		v8::Platform* platform = v8::platform::CreateDefaultPlatform();
		v8::V8::InitializePlatform(platform);
		v8::V8::Initialize();
		isolate = v8::Isolate::New();
		SCRIPT_INITIALIZED = 1;
	}
#else
	OPlog("V8 Engine Feature not enabled.");
#endif
}


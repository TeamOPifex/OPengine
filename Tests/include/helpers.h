#ifndef OP_ENGINE_TEST_HELPERS
#define OP_ENGINE_TEST_HELPERS
#include <Core/include/Core.h>
#include <stdio.h>

#define OP_TEST_INIT OPint OP_TEST_COUNT = 1;

extern OPint OP_TEST_COUNT;

void OP_RTMSG(const OPchar* format, ...){
	OPchar buf[1024];
	va_list args;
    
    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);

	OPlog("\t%s", buf);

    return;
}

OPint OP_TEST(OPint (*testRoutine)(void*), const OPchar* name, void* args){
	OPint result = 0;
	OPlog("#%d %s - Running", OP_TEST_COUNT++, name);
	result = testRoutine(args);

	if(result){
		OPlog("\t%s - \033[1;31mFAILED\033[0m with %d\n", name, result);
	}
	else{
		OPlog("\t%s - \033[0;32mPASSED\033[0m with %d\n", name, result);
	}

	return result;
}

#endif
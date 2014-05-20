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

void OP_HEX_DUMP(void *addr, OPint len) {
    OPint i;
    ui8 buff[17];
    ui8 *pc = (ui8*)addr;
	ui8 desc[1024];

    // Output description if given.
    if (desc != NULL)
        printf ("%s:\n", desc);

    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf ("\t%s\n", buff);

            // Output the offset.
            printf ("  %04x ", i);
        }

        // Now the hex code for the specific character.
        printf (" %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }
}
#endif
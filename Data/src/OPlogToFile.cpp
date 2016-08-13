#include "./Data/include/OPlogToFile.h"

#include "./Data/include/OPfile.h"
#include "./Core/include/OPlog.h"

OPfileInformation _logFile;

void _OPlogToFile(ui32 level, const char* channel, const char* message) {
	char buffer[1024];
    #ifdef OPIFEX_WINDOWS
	sprintf_s(buffer, 1024, "%s[%d]: %s\n", channel, level, message);
    #else
	sprintf(buffer, "%s[%d]: %s\n", channel, level, message);
    #endif
	fprintf(_logFile.file, "%s", buffer);
}

void OPlogToFile(const char* path) {
	_logFile = OPfile::Create(path);
	OPlogHandler = _OPlogToFile;
}

void OPlogToFileClose() {
	if (_logFile.file == NULL) return;
	fclose(_logFile.file);
}

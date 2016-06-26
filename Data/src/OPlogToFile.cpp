#include "./Data/include/OPlogToFile.h"
#include "./Data/include/OPfile.h"

OPfileInformation _logFile;

void _OPlogToFile(ui32 level, const char* channel, const char* message) {
	char buffer[1024];
	sprintf(buffer, "%s[%d]: %s\n", channel, level, message);
	fprintf(_logFile.file, "%s", buffer);
}

void OPlogToFile(const char* path) {
	_logFile = OPfileCreate(path);
	OPlogHandler = _OPlogToFile;
}

void OPlogToFileClose() {
	fclose(_logFile.file);
}
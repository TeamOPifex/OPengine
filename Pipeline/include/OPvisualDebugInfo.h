#pragma once

struct OPvisualDebugInfo;
typedef struct OPvisualDebugInfo OPvisualDebugInfo;

extern OPvisualDebugInfo OPVISUALDEBUGINFO;

#include "./Core/include/OPmemory.h"
#include "./Core/include/OPtimer.h"

struct OPvisualDebugInfo {
	void Init();
    void PreInitSetup();
	void Update(OPtimer* timer);
	void Render(OPfloat delta);
	void Destroy();

	inline static OPvisualDebugInfo* Create() {
		return OPNEW(OPvisualDebugInfo());
	}
};

#pragma once
#ifndef OPEngine_Core
#define OPEngine_Core

#include "Target.h" // defines the current build target
#include "Types.h"
#include "GameCycle.h"
#include "DynamicMemory.h"
#include "Timer.h"
#include "MathHelpers.h"

// prevent name mangling if compiling with c++
#ifdef __cplusplus
namespace OPEngine{
namespace Core{
class OPCore{
	public: 
		OPCore(){}
		void Start();
		void End();
	private:
		OPint _OPengineRunning;
		
		virtual void Initialize() = 0;
		virtual void Update(OPtimer* timer) = 0;
		virtual void Destroy() = 0;
};
}
}
#else
//---- Function prototypes ---------------------------------------------------
/**
 * OPstart - Begins the game cycle.
 *	This function is responsible for several actions. It creates an
 *	OPtimer instance, and invokes the function pointer OPinitialize() to
 *	perform user defined initializations. Once initialized, the game loop
 *	is started. The timer instance is updated on each iteration and passed
 *	to the OPupdate function pointer. The game loop runs until OPend() is
 *	called. At which point the OPdestroy() function pointer is called and
 *	and clean up is performed.
 */
void OPstart();
//----------------------------------------------------------------------------
/**
 * OPend - Ends the game cycle.
 *	This function sets an internal flag which will cease the game loop.
 *	and result in the termination and clean up of all user code and 
 *	data. 
 */
void OPend();
#endif
#endif
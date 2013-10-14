#pragma once
#ifndef OPEngine_Core_GameCycle
#define OPEngine_Core_GameCycle
#include "Timer.h"

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif
/**
 * OPinitialize - Game engine initialization.
 *	This function pointer points to a user function which is responsible
 *	for performing any essential initialization, data allocation and setup
 *	which is needed to begin the primary game loop.
 */
extern void (*OPinitialize)();
//----------------------------------------------------------------------------
/**
 * OPupdate - Game engine update.
 *	This function pointer points to a user function which accepts an
 *	OPtimer as an argument. This function performs whatever action needs
 *	to be taken to actually run the game. For example, this function would
 *	be responsible for invoking game-logic updates, input device polling
 *	and scene rendering.
 * @param timer OPtimer object representing current time measurments
 */
extern int(*OPupdate)(OPtimer*);
//----------------------------------------------------------------------------
/**
 * OPdestroy - Game engine termination.
 *	This function pointer points to a user function which is responsible
 *	for gracefully terminating the game engine. This may include saving
 *	data, deallocating memory, releasing OS resources and closing
 *	network connections.
 */
extern void (*OPdestroy)();
#ifdef __cplusplus
};
#endif
#endif

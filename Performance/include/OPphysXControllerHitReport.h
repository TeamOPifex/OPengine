#ifndef OP_PERFORMANCE_PHYSXCONTROLLERHITREPORT
#define OP_PERFORMANCE_PHYSXCONTROLLERHITREPORT

#include "./Performance/include/OPphysX.h"

#ifdef OPIFEX_OPTION_PHYSX

class OPphysXControllerHitReport : public PxUserControllerHitReport 
{
public:
	/**
	\brief Called when current controller hits a shape.

	This is called when the CCT moves and hits a shape. This will not be called when a moving shape hits a non-moving CCT.

	\param[in] hit Provides information about the hit.

	@see PxControllerShapeHit
	*/
	virtual void onShapeHit(const PxControllerShapeHit& hit) {
		if(_onShapeHit == NULL) return;
		_onShapeHit(hit);
	}

	/**
	\brief Called when current controller hits another controller.

	\param[in] hit Provides information about the hit.

	@see PxControllersHit
	*/
	virtual void onControllerHit(const PxControllersHit& hit) {
		OPlog("Controller Hit");
	}

	/**
	\brief Called when current controller hits a user-defined obstacle.

	\param[in] hit Provides information about the hit.

	@see PxControllerObstacleHit PxObstacleContext
	*/
	virtual void onObstacleHit(const PxControllerObstacleHit& hit) {

	}

	OPphysXControllerHitReport(	
		void(*onShapeHit)(PxControllerShapeHit),
		void(*onControllerHit)(PxControllersHit),
		void(*onObstacleHit)(PxControllerObstacleHit)) {
		_onShapeHit = onShapeHit;
		_onControllerHit = onControllerHit;
		_onObstacleHit = onObstacleHit;
	}
private:

	void(*_onShapeHit)(PxControllerShapeHit);
	void(*_onControllerHit)(PxControllersHit);
	void(*_onObstacleHit)(PxControllerObstacleHit);
};

#endif

#endif

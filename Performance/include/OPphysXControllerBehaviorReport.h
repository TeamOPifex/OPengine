#pragma once

#include "./Performance/include/OPphysX.h"

#ifdef OPIFEX_OPTION_PHYSX

class OPphysXControllerBehaviorReport : public PxControllerBehaviorCallback {

public:
	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxShape& shape, const PxActor& actor) {
		if (_onShapeHit == NULL) return PxControllerBehaviorFlag::eCCT_SLIDE;
		return _onShapeHit(shape, actor);
	}

	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxController& controller) {
		return PxControllerBehaviorFlag::eCCT_SLIDE;
	}

	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxObstacle& obstacle) {
		return PxControllerBehaviorFlag::eCCT_SLIDE;
	}

	OPphysXControllerBehaviorReport(
		PxControllerBehaviorFlags(*onShapeHit)(const PxShape&, const PxActor&)) {
		_onShapeHit = onShapeHit;
	}
private:
	PxControllerBehaviorFlags(*_onShapeHit)(const PxShape& shape, const PxActor& actor);
};

#endif
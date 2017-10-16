/*
 * TargetIsInLineOfSightRayCastCallback.cpp
 *
 *  Created on: Dec 5, 2014
 *      Author: DCurro
 */

#include "TargetIsInLineOfSightRayCastCallback.hpp"

#include "common.hpp"

TargetIsInLineOfSightRayCastCallback::TargetIsInLineOfSightRayCastCallback(PooledPlayEntity* target) : mTarget(target) {
}

TargetIsInLineOfSightRayCastCallback::~TargetIsInLineOfSightRayCastCallback() {
}

bool TargetIsInLineOfSightRayCastCallback::targetIsInDirectLineOfSight() {
	return mTargetIsInDirectLineOfSight;
}

float32 TargetIsInLineOfSightRayCastCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {
	UNUSED(point);
	UNUSED(normal);

	if(fixture->IsSensor()) {
		return -1.0f;
	}

	if(fixture->GetBody()->GetUserData() == mTarget) {
		mTargetIsInDirectLineOfSight = true;
		mFraction = fraction;
		return fraction;
	} else {
		mTargetIsInDirectLineOfSight = false;
		return fraction;
	}
}

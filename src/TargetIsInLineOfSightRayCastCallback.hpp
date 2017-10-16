/*
 * TargetIsInLineOfSightRayCastCallback.hpp
 *
 *  Created on: Dec 5, 2014
 *      Author: DCurro
 */

#ifndef TARGETISINLINEOFSIGHTRAYCASTCALLBACK_HPP_
#define TARGETISINLINEOFSIGHTRAYCASTCALLBACK_HPP_

#include <Box2D/Box2D.h>

class PooledPlayEntity;

class TargetIsInLineOfSightRayCastCallback : public b2RayCastCallback {
private:
	PooledPlayEntity* mTarget;
	float32 mFraction = 1.0f;

	bool mTargetIsInDirectLineOfSight = false;

	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction);

public:
	TargetIsInLineOfSightRayCastCallback(PooledPlayEntity* target);
	~TargetIsInLineOfSightRayCastCallback();

	bool targetIsInDirectLineOfSight();
};

#endif /* TARGETISINLINEOFSIGHTRAYCASTCALLBACK_HPP_ */

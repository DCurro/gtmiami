/*
 * ExplosionCircleCaster.cpp
 *
 *  Created on: Dec 5, 2014
 *      Author: DCurro
 */

#include "ExplosionCircleCaster.hpp"

#include "PlayContext.hpp"

ExplosionCircleCaster::ExplosionCircleCaster(PlayContext* playContext, int floor, float radius, b2Vec2 worldCenterOfBlast) {
	mPlayContext = playContext;
	mFloor = floor;
	mRadius = radius;
	mWorldCenterOfBlast = worldCenterOfBlast;
}

ExplosionCircleCaster::~ExplosionCircleCaster() {
}

std::vector< std::pair<PooledPlayEntity*, float> > ExplosionCircleCaster::getEntitiesAffectedByExplosion() {
	return this->mEntitiesAffectedByExplosion;
}

void ExplosionCircleCaster::cast() {
	CircleCallback callback(mPlayContext, mFloor, mRadius, mWorldCenterOfBlast);

	b2AABB aabb;
	aabb.upperBound = mWorldCenterOfBlast + b2Vec2(mRadius,mRadius);
	aabb.lowerBound = mWorldCenterOfBlast - b2Vec2(mRadius,mRadius);

	mPlayContext->getWorld()->QueryAABB(&callback, aabb);

	mEntitiesAffectedByExplosion = callback.getAffectedEntities();
}

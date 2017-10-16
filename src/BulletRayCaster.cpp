#include "BulletRayCaster.hpp"

#include <iostream>

#include "PlayContext.hpp"
#include "PlayEntity.hpp"

BulletRayCaster::BulletRayCaster(){
	mPlayEntityStruct = NULL;
	mFraction = 1.0f;
}

BulletRayCaster::~BulletRayCaster() {
}

void BulletRayCaster::castRay(PlayContext* playContext, int floor, b2Vec2 initialPoint, float angleInDegrees, float shotDistance) {
	float xOffset = std::cos(angleInDegrees*DEGREES_TO_RADIANS) * shotDistance + initialPoint.x;
	float yOffset = std::sin(angleInDegrees*DEGREES_TO_RADIANS) * shotDistance + initialPoint.y;
	b2Vec2 finalPosition(xOffset, yOffset);

	BulletCastCallback callback(floor);
	playContext->getWorld()->RayCast(&callback, initialPoint, finalPosition);

	mPlayEntityStruct = callback.getEntityStruck();
	mPointOfIntersection = callback.getPointOfIntersection();
	mNormal = callback.getNormal();
	mFraction = callback.getFraction();
	if(mFraction == 0)  {
		mFraction = 1.0f;
	}

}

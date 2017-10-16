#ifndef BULLETRAYCASTER_HPP_
#define BULLETRAYCASTER_HPP_

#include <Box2D/Box2D.h>

#include "common.hpp"
#include "PlayEntity.hpp"

class PlayContext;
class PlayEntity;

class BulletRayCaster {
protected:
	GETTER_AND_DEFAULT_NULL(PlayEntity*, PlayEntityStruct);
	GETTER(b2Vec2, PointOfIntersection);
	GETTER(b2Vec2, Normal);
	GETTER(float, Fraction);

public:
	BulletRayCaster();
	virtual ~BulletRayCaster();

	//casts the ray and returns the first object struck by the cast.  Returns NULL if no objects interacted with the cast.
	void castRay(PlayContext* playContext, int floor, b2Vec2 initialPoint, float angleInDegrees, float shotDistance);

protected:
	class BulletCastCallback : public b2RayCastCallback {
	protected:
		GETTER_AND_DEFAULT_NULL(PlayEntity*, EntityStruck);
		GETTER(b2Vec2, PointOfIntersection);
		GETTER(b2Vec2, Normal);
		GETTER(float, Fraction);
		GETTER(int, Floor);

		float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {
			PlayEntity* entity = NULL;
			if(fixture->GetUserData() != NULL){
				entity = (PlayEntity*)fixture->GetUserData();
			} else {
				entity = (PlayEntity*)fixture->GetBody()->GetUserData();
			}

			if(fixture->IsSensor()) {
				return -1; //ignore sensors
			}

			if(entity==NULL) {
				return -1;
			}

			if(!entity->isOnFloor(mFloor)) {
				return -1;
			}

			mEntityStruck = entity;
			mPointOfIntersection = point;
			mNormal = normal;
			mFraction = fraction;

			return fraction;
		}

	public:
		BulletCastCallback(int floor) {
			mFloor = floor;

			mFraction = 0.0f;
		}
	};
};

#endif /* BULLETRAYCASTER_HPP_ */

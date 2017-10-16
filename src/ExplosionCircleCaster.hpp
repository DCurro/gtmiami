#ifndef EXPLOSIONCIRCLECASTER_HPP_
#define EXPLOSIONCIRCLECASTER_HPP_

#include <Box2D/Box2D.h>

#include "common.hpp"
#include "PlayEntity.hpp"
#include "PooledPlayEntity.hpp"
#include "ClassChecker.hpp"
#include "BulletRayCaster.hpp"

class PlayContext;
class PooledPlayEntity;

class ExplosionCircleCaster {
private:
	PlayContext* mPlayContext;
	int mFloor;
	float mRadius;
	b2Vec2 mWorldCenterOfBlast;
	std::vector< std::pair<PooledPlayEntity*, float> > mEntitiesAffectedByExplosion;

public:
	ExplosionCircleCaster(PlayContext* playContext, int floor, float radius, b2Vec2 worldCenterOfBlast);
	virtual ~ExplosionCircleCaster();

	virtual void cast();

	//returns the entities who are in line of sight of the explosion, as well as their ratio from the blast (0 being not at all to 1 center of blast)
	virtual std::vector< std::pair<PooledPlayEntity*, float> > getEntitiesAffectedByExplosion();

private:
	class CircleCallback : public b2QueryCallback {
	protected:
		PlayContext* mPlayContext;
		int mFloor;
		float mRadius;
		b2Vec2 mWorldCenterOfBlast;

		std::set< std::pair<PooledPlayEntity*,float> > mAffectedEntities;

	public:
		CircleCallback(PlayContext* playContext, int floor, float radius, b2Vec2 worldCenterOfBlast) {
			mPlayContext = playContext;
			mFloor = floor;
			mRadius = radius;
			mWorldCenterOfBlast = worldCenterOfBlast;
		}

		std::vector< std::pair<PooledPlayEntity*,float> > getAffectedEntities() {
			return std::vector< std::pair<PooledPlayEntity*,float> >(mAffectedEntities.begin(), mAffectedEntities.end() );
		}

		bool ReportFixture(b2Fixture* fixture) {
			//returning true means to keep looking for fixtures.
			if(fixture->IsSensor()) {
				return true;
			}

			//TODO: check for same floor

			PlayEntity* entity = NULL;
			if(fixture->GetUserData() != NULL){
				entity = (PlayEntity*)fixture->GetUserData();
			} else {
				entity = (PlayEntity*)fixture->GetBody()->GetUserData();
			}

			if(entity == NULL) {
				return true;
			}

			PooledPlayEntity* pooledPlayEntity = NULL;
			if(!ClassChecker().isKindOfClass<PooledPlayEntity*>(entity)){
				return true;
			}
			pooledPlayEntity = dynamic_cast<PooledPlayEntity*>(entity);

			BulletRayCaster bulletCast;

			b2Vec2 vector1 = mWorldCenterOfBlast;
			b2Vec2 vector2 = entity->getBody()->GetPosition();
            
			float radianAngle = atan2(vector2.y-vector1.y, vector2.x-vector1.x);
            
            if (radianAngle < 0) {
                radianAngle += 2.0f * PI_CONSTANT;
            }
            
			float degreeAngle = radianAngle * RADIANS_TO_DEGREES;

			bulletCast.castRay(mPlayContext, mFloor, mWorldCenterOfBlast, degreeAngle, mRadius);

			if(entity == bulletCast.getPlayEntityStruct()) {
				float ratioFromBlast = (1.0f-bulletCast.getFraction()>0.8f) ? 1.0f : 1.0f-bulletCast.getFraction();
				this->mAffectedEntities.insert(std::pair<PooledPlayEntity*,float>(pooledPlayEntity,ratioFromBlast));
			}

			return true;
	  }
	};
};

#endif /* EXPLOSIONCIRCLECASTER_HPP_ */

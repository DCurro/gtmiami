#include "TurretMind.hpp"

#include <iostream>

#include "PlayContext.hpp"
#include "PooledPlayEntity.hpp"
#include "Animal.hpp"
#include "TargetIsInLineOfSightRayCastCallback.hpp"

TurretMind::TurretMind() {
}

TurretMind::~TurretMind() {
}

void TurretMind::setup(PlayContext* playContext, Animal* controllingAnimal) {
	Mind::setup(playContext, controllingAnimal);

	mFaceAngle = std::rand() % 360;
}

void TurretMind::update(float timeStep) {
	Mind::update(timeStep);

	mFaceAngle += mAnimal->motionSpeed() * timeStep;
	if(mFaceAngle >= 360.0f) {
		mFaceAngle = mFaceAngle - 360.0f;
	}

	mAnimal->applyLook(mFaceAngle);

	if(this->isValid(mTargetEntity)){
		PooledPlayEntity* targetEntity = mTargetEntity;

		TargetIsInLineOfSightRayCastCallback callback(targetEntity);
		this->getPlayContext()->getWorld()->RayCast(&callback, mAnimal->getBody()->GetPosition(), targetEntity->getBody()->GetPosition());

		if( callback.targetIsInDirectLineOfSight() ){
			b2Vec2 position = targetEntity->getBody()->GetPosition();
			b2Vec2 differenceVec = position - mAnimal->getBody()->GetPosition();
			mFaceAngle = angleInDegreesFromVector(differenceVec);
			mAnimal->applyBeginAttack(mFaceAngle);
		} else {
			mAnimal->applyEndAttack(mFaceAngle);
		}
	} else {
		mAnimal->applyEndAttack(mFaceAngle);
	}
}

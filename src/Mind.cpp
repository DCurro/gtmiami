#include "Mind.hpp"

#include "Animal.hpp"
#include "InputHandler.hpp"
#include "PlayContext.hpp"
#include "GameContext.hpp"

#include <iostream>

Mind::Mind() {
	mAnimal = NULL;
}

Mind::~Mind() {
}

void Mind::setup(PlayContext* playContext, Animal* animal) {
    // TODO: figure out if minds need a floor...
	PlayEntity::setup(playContext, 0);

	mPlayContext = playContext;
	mAnimal = animal;
	this->registerPooledPlayEntity(mAnimal);
	mTargetEntity = NULL;


}

void Mind::update(float timeStep) {
	PlayEntity::update(timeStep);

	if( !this->isValid(mAnimal) ) {
		return;
	}
}

void Mind::setTargetEntity(PooledPlayEntity* targetEntity){
	if(targetEntity){
		mTargetEntity = targetEntity;
		this->registerPooledPlayEntity(mTargetEntity);
	} else {
		mTargetEntity = NULL;
	}
}

PooledPlayEntity* Mind::getTargetEntity() {
	if(this->isValid(mTargetEntity)){
		return mTargetEntity;
	}

	return NULL;
}

void Mind::hearSoundAtLocation(b2Vec2 soundLocation) {
    UNUSED(soundLocation);
}

b2Body* Mind::getBody() {
    assert(false && "a mind do not have body; it has an animal");
}

// PlayEntity

Animation* Mind::newBulletStruckAnimation() {
	return NULL;
}

void Mind::struckByBulletConsequences(float bulletCalibre) {
	UNUSED(bulletCalibre);
}

void Mind::struckByBlastConsequences(b2Vec2 blastForce, float damage) {
	UNUSED(blastForce);
	UNUSED(damage);
}

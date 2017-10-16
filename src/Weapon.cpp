/*
 * Weapon.cpp
 *
 *  Created on: Dec 9, 2014
 *      Author: DCurro
 */

#include "Weapon.hpp"

#include <vector>

#include "InputHandler.hpp"
#include "PlayContext.hpp"
#include "GameContext.hpp"
#include "ResourceManager.hpp"
#include "AudioManager.hpp"
#include "Animation.hpp"
#include "ExplosionCircleCaster.hpp"
#include "ClassChecker.hpp"
#include "Animal.hpp"
#include "CircleAnimalFinderQueryCallback.hpp"

Weapon::Weapon() {

}

Weapon::~Weapon() {

}

void Weapon::setup(PlayContext* playContext, int persistentIdNumber, int floor) {
	PooledPlayEntity::setup(playContext, persistentIdNumber, floor);

	mAccumulatedCooldownTime = this->cooldownTimeInSeconds();

	mIsHeld = false;

	mIsStrikingWithWeapon = false;
	mJustFinishedStrikingWithWeapon = false;
	mStrikingAngle = 0.0f;
	mCanAttackWithWeapon = true;
}

void Weapon::startStrikingWithWeapon(float angle) {
	mStrikingAngle = angle;

	mIsStrikingWithWeapon = true;
}

void Weapon::changeAngle(float angle) {
	mStrikingAngle = angle;
}

void Weapon::stopStrikingWithWeapon(float angle) {
    UNUSED(angle);
    
	if(mIsStrikingWithWeapon) {
		mJustFinishedStrikingWithWeapon = true;
	}
	mIsStrikingWithWeapon = false;

	mCanAttackWithWeapon = true;
}

void Weapon::strike(float angle) {
	if(mAccumulatedCooldownTime < this->cooldownTimeInSeconds()) {
		return;
	}

    //TODO: don't use > 0 ... use a NONE enum or something nicer...
	if( this->actionSoundEffect() >= 0 ){
		mPlayContext->getGameContext()->getAudioManager()->play( this->actionSoundEffect() );
        
        this->informNearbyAnimalsOfSound();
	}

	mAccumulatedCooldownTime = 0.0f;

	this->strikeWithAngle(angle);
}

void Weapon::update(float timeStep) {
	PooledPlayEntity::update(timeStep);
    
	if(mAccumulatedCooldownTime <= this->cooldownTimeInSeconds() ){
		mAccumulatedCooldownTime += timeStep;
	}

	if(this->strikeAtEndOfUse()) {
		if(mJustFinishedStrikingWithWeapon) {
			this->strike(mStrikingAngle);
		}
	} else {
		if(mIsStrikingWithWeapon) {
			this->strike(mStrikingAngle);
		}
	}
    
    mJustFinishedStrikingWithWeapon = false;

	if(this->idleAnimation()) {
		this->idleAnimation()->update(timeStep);
		mSprite.setTexture(*this->idleAnimation()->getCurrentFrame(), true);
	}
}

void Weapon::draw(sf::RenderTarget& window) {
	if(!mIsHeld) {
		PooledPlayEntity::draw(window);
	}
}

// PlayEntity

Animation* Weapon::newBulletStruckAnimation() {
	return NULL;
}

void Weapon::struckByBulletConsequences(float bulletCalibre) {
	UNUSED(bulletCalibre);
}

void Weapon::struckByBlastConsequences(b2Vec2 blastForce, float damage) {
	UNUSED(blastForce);
	UNUSED(damage);
}

#pragma Private Methods

void Weapon::informNearbyAnimalsOfSound() {
    if (this->soundRadius() > 0) {
        CircleAnimalFinderQueryCallback animalFinderCallback(mPlayContext, mCurrentFloor, this->soundRadius(), this->getBody()->GetPosition());
        
        std::vector<Animal *> animalsThatHearSound = animalFinderCallback.queryWorldAndGetAnimals();
        
        for (Animal* animal : animalsThatHearSound) {
            animal->hearWeaponSoundAtLocation(this->mBody->GetPosition());
            std::cout << animal->getDebugName() << " heard a sound" << std::endl;
        }
    }
}
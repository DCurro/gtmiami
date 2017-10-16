/*
 * Spirit.cpp
 *
 *  Created on: Dec 8, 2014
 *      Author: DCurro
 */

#include "Spirit.hpp"

#include "GameContext.hpp"
#include "PlayContext.hpp"
#include "ResourceManager.hpp"
#include "Animation.hpp"
#include "RandomIdentifierProvider.hpp"

Spirit::Spirit() {
	mIdleAnimation = NULL;
}

Spirit::~Spirit() {
}

void Spirit::setup(PlayContext* playContext, int floor) {
	Person::setup(playContext, floor);

	mIdleAnimation = new Animation(true,
								   4.0f,
								   mPlayContext->getGameContext()->getResourceManager()->getImage(ResourceManager::ImageResources::spiritWalkCycle_png));
}

void Spirit::prepareForReuse() {
	Person::prepareForReuse();

	if(mIdleAnimation) {
		delete mIdleAnimation;
	}
	mIdleAnimation = NULL;
}

void Spirit::update(float timeStep){
	Person::update(timeStep);

	this->getBody()->SetTransform(this->getBody()->GetPosition(), mLookAngle * RADIANS_TO_DEGREES);

	b2Vec2 vel = this->getBody()->GetLinearVelocity();

	float desiredXVelocity = mMotionDirection.x*mMotionPower*this->motionSpeed();
	float xVelocityChange = desiredXVelocity - vel.x;
	float xImpulse = this->getBody()->GetMass() * xVelocityChange;

	float desiredYVelocity = mMotionDirection.y*mMotionPower*this->motionSpeed();
	float yVelocityChange = desiredYVelocity - vel.y;
	float yImpulse = this->getBody()->GetMass() * yVelocityChange;

	this->getBody()->ApplyLinearImpulse( b2Vec2(xImpulse,yImpulse), this->getBody()->GetWorldCenter(), true);

	mMotionDirection = b2Vec2(0,0);
	mMotionPower = 0.0f;
}

//<Animal>

float Spirit::motionSpeed() {
	return 4.0f;
}

float Spirit::maxHealthPoints() {
	return 10.0f;
}

Animation* Spirit::idleAnimation() {
	return mIdleAnimation;
}

Animation* Spirit::walkAnimation() {
	return mIdleAnimation;
}

Animation* Spirit::deadAnimation() {
	return mIdleAnimation;
}

void Spirit::hearWeaponSoundAtLocation(b2Vec2 location) {
    UNUSED(location);
}

//<Inspectable>

std::vector<ItemDescription> Spirit::getDebugDescription() {
	std::vector<ItemDescription> itemDescriptionList;

	std::stringstream ss;
	if( mPersistentIdNumber == NO_PERSISTENCE_ID) {
        ss << "persistence id: " << "NON-PERSISTENT OBJECT" << std::endl;
    } else {
        ss << "persistence id: " << mPersistentIdNumber << std::endl;
    }
	ItemDescription idDescription(ss.str(), NULL, ItemDescription::UsageType::NO_USAGE);
	itemDescriptionList.push_back(idDescription);

	return itemDescriptionList;
}

std::string Spirit::getDebugName() {
	return "Spirit";
}


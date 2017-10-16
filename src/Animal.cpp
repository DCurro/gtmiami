#include "Animal.hpp"

#include <typeinfo>

#include "common.hpp"
#include "Automobile.hpp"
#include "Animation.hpp"
#include "Weapon.hpp"
#include "AuxiliaryWeapon.hpp"
#include "Gun.hpp"
#include "PlayContext.hpp"
#include "GameContext.hpp"
#include "ResourceManager.hpp"
#include "Factory.hpp"
#include "EntityManager.hpp"
#include "Spirit.hpp"
#include "Mind.hpp"
#include "ClassChecker.hpp"
#include "DriverBody.hpp"

Animal::Animal() : PooledPlayEntity() {
	this->mLocalVehiclesMultiSet = NULL;

	this->mMind = NULL;

	mVisionSensorFixture = NULL;
	mVisionSet = NULL;

	mHealthPoints = 0.0f;

	mCurrentAnimaton = NULL;

	this->mBodyFixture = NULL;

	mMotionDirection = b2Vec2(0,0);
	mMotionPower = 0.0f;
	mLookAngle = 0.0f;

	mPerformedBeginAttack = true;
	mWeaponsInPickupRange = new std::vector<Weapon*>();

	mIsDriving = false;

	mCurrentWeapon = NULL; //TODO: gun is a pooled play entity... register it, don't just use it!
}

Animal::~Animal() {
	this->prepareForReuse();
}

void Animal::setup(PlayContext* playContext, int floor) {
    UNUSED(playContext);
    UNUSED(floor);
    
    assert(false); // This is an Animal, use the appropriate setup method that sets a Mind*.
}

void Animal::setup(PlayContext* playContext, int persistentIdNumber, Mind* mind, int floor) {
	PooledPlayEntity::setup(playContext, persistentIdNumber, floor);

	mMind = mind;

	this->mLocalVehiclesMultiSet = new std::multiset<Automobile*>;

	mVisionSensorFixture = NULL;
	mVisionSet = new std::set<PlayEntity*>();

	mHealthPoints = this->maxHealthPoints();

	mCurrentAnimaton = NULL;

	this->mBodyFixture = NULL;

	mMotionDirection = b2Vec2(0,0);
	mMotionPower = 0.0f;
	mLookAngle = 0.0f;

	mPerformedSwitchWeapon = false;
	mPerformedBeginAttack = true;
	mPerformedEndAttack = false;
	mWeaponsInPickupRange = new std::vector<Weapon*>();

	mIsDriving = false;

	mCurrentWeapon = NULL; //TODO: gun is a pooled play entity... register it, don't just use it!
	mGun = NULL;
	mAuxiliaryWeapon = NULL;
}

void Animal::prepareForReuse() {
	PooledPlayEntity::prepareForReuse();

	if(mLocalVehiclesMultiSet) {
		delete mLocalVehiclesMultiSet;
	}
	mLocalVehiclesMultiSet = NULL;

	mMind = NULL;

	mVisionSensorFixture = NULL;
	if(mVisionSet) {
		delete mVisionSet;
	}
	mVisionSet = NULL;

	if(mWeaponsInPickupRange) {
		delete mWeaponsInPickupRange;
	}
	mWeaponsInPickupRange = NULL;

	mCurrentWeapon = NULL;
}

void Animal::update(float timeStep) {
	PooledPlayEntity::update(timeStep);

	if(mMind && mHealthPoints > 0){
		mMind->update(timeStep);
	}

	mCurrentAnimaton = NULL;

	if(mHealthPoints <= 0){
		if(this->deadAnimation() != NULL) {
			mCurrentAnimaton = this->deadAnimation();
		}
		this->mBody->SetActive(false);
        
	} else if(mMotionDirection.x != 0 || mMotionDirection.y !=0){
		if(this->walkAnimation() != NULL) {
			mCurrentAnimaton = this->walkAnimation();
		}
        
	} else {
		if(this->idleAnimation() != NULL) {
			mCurrentAnimaton = this->idleAnimation();
		}
	}

	if(mCurrentAnimaton) {
		mCurrentAnimaton->update(timeStep);
		mSprite.setTexture(*mCurrentAnimaton->getCurrentFrame(), true);
	}

	if(mHealthPoints <= 0 ){
//		if(mMind) {
//			Spirit* releasedSpirit = this->mPlayContext->getFactory()->createSpirit(mPlayContext, mCurrentFloor, mMind, this->getBody()->GetPosition(), this->getBody()->GetAngle());
//			this->mPlayContext->getEntityManager()->addEntity(releasedSpirit);
//			mMind = NULL;
//		}

		return;
	}

    this->getBody()->SetTransform(this->getBody()->GetPosition(), mLookAngle * DEGREES_TO_RADIANS);
    
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

	if(mPerformedBeginAttack == true) {
		if(this->isValid(mCurrentWeapon)) {
			mCurrentWeapon->startStrikingWithWeapon(mLookAngle);
		}
	}
	mPerformedBeginAttack = false;

	if(mPerformedEndAttack == true) {
		if(this->isValid(mCurrentWeapon)) {
			mCurrentWeapon->stopStrikingWithWeapon(mLookAngle);
		}
	}
	mPerformedEndAttack = false;


	if(mPerformedSwitchWeapon == true) {
		if(ClassChecker().isKindOfClass<Gun*>(mCurrentWeapon)) {
			mCurrentWeapon = mAuxiliaryWeapon;
		} else if(ClassChecker().isKindOfClass<AuxiliaryWeapon*>(mCurrentWeapon)) {
			mCurrentWeapon = mGun;
		}
	}
	mPerformedSwitchWeapon = false;
    
	if(this->isValid(mGun)) {
		mGun->getBody()->SetTransform(this->getBody()->GetPosition(), this->getBody()->GetAngle());
		mGun->changeAngle(mLookAngle);
		mGun->setCurrentFloor(this->getCurrentFloor());
	}
	if(this->isValid(mAuxiliaryWeapon)) {
		mAuxiliaryWeapon->getBody()->SetTransform(this->getBody()->GetPosition(), this->getBody()->GetAngle());
		mAuxiliaryWeapon->changeAngle(mLookAngle);
		mAuxiliaryWeapon->setCurrentFloor(this->getCurrentFloor());
	}

}

void Animal::draw(sf::RenderTarget &window) {
    PooledPlayEntity::draw(window);
    
    if (mMind) {
        mMind->draw(window);
    }
}

void Animal::pickUpClosestWeapon() {
	Weapon* previousCurrentWeapon = mCurrentWeapon;
	Gun* previousGun = mGun;
	AuxiliaryWeapon* previousAuxiliaryWeapon = mAuxiliaryWeapon;

	Weapon* closestWeapon = NULL;
	float lastDistance = INT32_MAX;

	for(auto weapon : *mWeaponsInPickupRange) {
		if(weapon->getIsHeld()) {
			continue;
		}

		float distanceToGun = b2Distance(this->getBody()->GetPosition(), weapon->getBody()->GetPosition());
		if( distanceToGun < lastDistance ) {
			lastDistance = distanceToGun;
			closestWeapon = weapon;
		}
	}

	if(closestWeapon) {
		mCurrentWeapon = closestWeapon;
		this->registerPooledPlayEntity(mCurrentWeapon);

		closestWeapon->setIsHeld(true);
		mWeaponsInPickupRange->erase(std::remove(mWeaponsInPickupRange->begin(), mWeaponsInPickupRange->end(), closestWeapon), mWeaponsInPickupRange->end());

		if(ClassChecker().isKindOfClass<Gun*>(closestWeapon)) {
			mGun = dynamic_cast<Gun*>(closestWeapon);
		} else if(ClassChecker().isKindOfClass<AuxiliaryWeapon*>(closestWeapon)) {
			mAuxiliaryWeapon = dynamic_cast<AuxiliaryWeapon*>(closestWeapon);
		} else {
			assert(false && "Unknown weapon type");
		}
	} else {
		if(ClassChecker().isKindOfClass<Gun*>(mCurrentWeapon)) {
			mGun = NULL;
		} else if(ClassChecker().isKindOfClass<AuxiliaryWeapon*>(mCurrentWeapon)) {
			mAuxiliaryWeapon = NULL;
		}

		if(mCurrentWeapon) {
			mCurrentWeapon->setIsHeld(false);
			mCurrentWeapon = NULL;
		}
	}

	if(this->isValid(previousCurrentWeapon)) {
		if( ClassChecker().isKindOfClass<Gun*>(mCurrentWeapon) && this->isValid(previousGun) ) {
			previousGun->setIsHeld(false);
			previousGun = dynamic_cast<Gun*>(mCurrentWeapon);
		} else if( ClassChecker().isKindOfClass<AuxiliaryWeapon*>(mCurrentWeapon) && this->isValid(previousAuxiliaryWeapon) ) {
			previousAuxiliaryWeapon->setIsHeld(false);
			previousAuxiliaryWeapon = dynamic_cast<AuxiliaryWeapon*>(mCurrentWeapon);
		}
	}
}

void Animal::applyMotion(b2Vec2 direction, float power) {
	mMotionDirection = direction;
	mMotionPower = power;
}

void Animal::applyLook(float angle) {
	mLookAngle = angle;
}

void Animal::applyBeginAttack(float angleInDegrees) {
	mPerformedBeginAttack = true;
	mLookAngle = angleInDegrees;
}

void Animal::applyEndAttack(float angleInDegrees) {
	mPerformedEndAttack = true;
	mLookAngle = angleInDegrees;
}

void Animal::applySwitchWeapon() {
	mPerformedSwitchWeapon = true;
}

void Animal::applyTalk(b2Vec2 direction) {
	UNUSED(direction);
}

void Animal::setMind(Mind* mind) {
	if(mind) {
		this->mMind = mind;
	}
}

void Animal::applyTryDriveClosestVehicle() {
	if (!mLocalVehiclesMultiSet->empty()) {
		Automobile* desiredAutomobile = NULL;
		Automobile* automobile = NULL;
		float deltaX = 0.0f;
		float deltaY = 0.0f;
		float squaredDistance = 0.0f;
		float minSquaredDistance = 0.0f;
		for (auto iterator = mLocalVehiclesMultiSet->begin();
				iterator != mLocalVehiclesMultiSet->end(); ++iterator) {
			automobile = dynamic_cast<Automobile*>(*iterator);
			if (automobile && !automobile->hasDriver()) {
				for (b2Vec2 vector : automobile->getDoorPositions()) {
					deltaX = vector.x - this->getBody()->GetPosition().x;
					deltaY = vector.y - this->getBody()->GetPosition().y;
					squaredDistance = (deltaX * deltaX) + (deltaY * deltaY);
					if (squaredDistance < 10) {
						if (desiredAutomobile) {
							if (minSquaredDistance > squaredDistance) {
								desiredAutomobile = automobile;
								minSquaredDistance = squaredDistance;
							}
						} else {
							desiredAutomobile = automobile;
							minSquaredDistance = squaredDistance;
						}
					}
					break; //TODO: why is there a break here?  Loop will only run once
				}
			}
		}
		if (desiredAutomobile) {
			if(ClassChecker().isKindOfClass<DriverBody*>(this)){
				DriverBody* thisDriverBody = dynamic_cast<DriverBody*>(this);
				desiredAutomobile->setDriver(thisDriverBody);
			}
		}
	}
}

// PlayEntity

Animation* Animal::newBulletStruckAnimation() {
	sf::Image* image = mPlayContext->getGameContext()->getResourceManager()->getImage(ResourceManager::ImageResources::bloodBulletStrikeAnimation_png);;
	return new Animation(false, 12, image);
}

void Animal::struckByBulletConsequences(float bulletCalibre) {
	mHealthPoints -= bulletCalibre;
}

void Animal::struckByBlastConsequences(b2Vec2 blastForce, float damage) {
	UNUSED(blastForce);

	mHealthPoints -= damage;
}

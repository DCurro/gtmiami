#include "Car.hpp"
#include "GameContext.hpp"

#include <sstream>
#include <set>

#include "PlayEntity.hpp"
#include "PlayContext.hpp"
#include "InputHandler.hpp"
#include "ResourceManager.hpp"
#include "EntityManager.hpp"
#include "DriverBody.hpp"
#include "RandomIdentifierProvider.hpp"

Car::Car() : Automobile() {
    mIdleAnimation = NULL;
}

Car::~Car() {
}

void Car::setup(PlayContext* playContext, int persistentIdNumber, int floor, b2Body* body, int teamID) {
    Automobile::setup(playContext, persistentIdNumber, floor);
    
    this->mBody = body;
    this->mTeamID = teamID;
    
    mIdleAnimation = new Animation(false, 0, mPlayContext->getGameContext()->getResourceManager()->getImage(ResourceManager::ImageResources::car_png));
}

//<Automobile>

float Car::maxForwardVelocity() {
    return 100.0f;
}

float Car::maxReverseVelocity() {
    return 30.0f;
}

float Car::maxForwardForce() {
    return 4000.0f;
}

float Car::maxBreakingForce() {
    return 10000.0f;
    
}

float Car::turnCoefficient() {
    return 1.0f;
}

float Car::maxImpulseBeforeDrifting() {
    return 300.0f;
}

float Car::dragAndTireFrictionWhileInDriveCoefficient() {
    return 2;
}

float Car::dragAndTireFrictionWhileParkedCoefficient() {
    return 10000;
}

float Car::angularImpulseWhileInDrive() {
    return 0.1f;
}

float Car::angularImpulseWhileParked() {
    return 2.0f;
}

Animation* Car::idleAnimation() {
    return mIdleAnimation;
}

//<Inspectable>

std::vector<ItemDescription> Car::getDebugDescription() {
    std::vector<ItemDescription> itemDescriptionList;
    
    std::stringstream ss;
    if( mPersistentIdNumber == NO_PERSISTENCE_ID) {
        ss << "persistence id: " << "NON-PERSISTENT OBJECT" << std::endl;
    } else {
        ss << "persistence id: " << mPersistentIdNumber << std::endl;
    }
    ItemDescription idDescription(ss.str(), NULL, ItemDescription::UsageType::NO_USAGE);
    itemDescriptionList.push_back(idDescription);
    
    //	std::function<void(bool)> f = std::bind(&Car::setIsParked);
    //	std::stringstream breakSs;
    //	breakSs << "is parked: " << (mIsParked ? "Yes" : "No") << std::endl;
    //	ItemDescription breakStateDescription(breakSs.str(), std::bind(&Car::setIsParked), ItemDescription::UsageType::BOOL);
    //	itemDescriptionList.push_back(breakStateDescription);
    
    return itemDescriptionList;
}

std::string Car::getDebugName() {
    return "Car";
}



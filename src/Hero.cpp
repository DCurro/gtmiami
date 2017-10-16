#include "Hero.hpp"

#include <sstream>

#include "common.hpp"
#include "InputHandler.hpp"
#include "PlayContext.hpp"
#include "Factory.hpp"
#include "GameContext.hpp"
#include "Camera.hpp"
#include "ResourceManager.hpp"
#include "Automobile.hpp"
#include "Mind.hpp"
#include "DriverMind.hpp"
#include "ActionTimer.hpp"
#include "Game.hpp"
#include "RandomIdentifierProvider.hpp"

Hero::Hero() : Person() {
    mWalkingAnimation = NULL;
    mIdleAnimation = NULL;
    mDeadAnimation = NULL;
}

Hero::~Hero() {
    this->prepareForReuse();
}

void Hero::setup(PlayContext* playContext, int persistentIdNumber, Mind* mind, int floor) {
    Person::setup(playContext, persistentIdNumber, mind, floor);
    
    mWalkingAnimation = new Animation(true,
                                      4.0f,
                                      mPlayContext->getGameContext()->getResourceManager()->getImage(ResourceManager::ImageResources::HeroWalkCycle_png));
    
    mIdleAnimation = new Animation(false,
                                   0.0f,
                                   mPlayContext->getGameContext()->getResourceManager()->getImage(ResourceManager::ImageResources::HeroIdleCycle_png));
    
    mDeadAnimation = new Animation(false,
                                   0.0f,
                                   mPlayContext->getGameContext()->getResourceManager()->getImage(ResourceManager::ImageResources::HeroDeadCycle_png));
}

void Hero::prepareForReuse() {
    Person::prepareForReuse();
    
    if(mWalkingAnimation) {
        delete mWalkingAnimation;
    }
    mWalkingAnimation = NULL;
    
    if(mIdleAnimation) {
        delete mIdleAnimation;
    }
    mIdleAnimation = NULL;
    
    if(mDeadAnimation) {
        delete mDeadAnimation;
    }
    mDeadAnimation = NULL;
}

void Hero::draw(sf::RenderTarget& window){
    if(!isValid(mAutomobileDriving)) {
        Person::draw(window);
    }
}

void Hero::update(float timeStep){
    Person::update(timeStep);
    
    if(isValid(mAutomobileDriving)){
        mAngle = mAutomobileDriving->getAngle();
        return;
    }
}

//<Animal>

float Hero::motionSpeed() {
    return 8.0f;
}

float Hero::maxHealthPoints() {
    return 10.0f;
}

Animation* Hero::idleAnimation() {
    return mIdleAnimation;
}

Animation* Hero::walkAnimation() {
    return mWalkingAnimation;
}

Animation* Hero::deadAnimation() {
    return mDeadAnimation;
}

void Hero::hearWeaponSoundAtLocation(b2Vec2 location) {
    UNUSED(location);
}

//<Driver>

void Hero::drive() {
    if(!isValid(mAutomobileDriving)){
        return;
    }
    
    DriverMind* driverMind = dynamic_cast<DriverMind*>(mMind);
    if(driverMind) {
        driverMind->drive();
    }
}

void Hero::willBeginDriving() {
    this->getBodyFixture()->SetSensor(true);
    mIsDriving = true;
    
    mPlayContext->getCamera()->setZoomLevel(2.0f);
}

void Hero::willEndDriving() {
    this->getBodyFixture()->SetSensor(false);
    mIsDriving = false;
    
    mPlayContext->getCamera()->setZoomLevel(1.0f);
}

void Hero::requestDropOffDriver() {
    if(mAutomobileDriving) {
        mAutomobileDriving->dropOffDriver();
    }
}

void Hero::requestApplyBreak() {
    if(mAutomobileDriving) {
        mAutomobileDriving->applyBreak();
    }
}

void Hero::requestApplyGas(float gasPower) {
    if(mAutomobileDriving) {
        mAutomobileDriving->applyGas(gasPower);
    }
}

void Hero::requestTurnWheel(float turnPower) {
    if(mAutomobileDriving) {
        mAutomobileDriving->turnWheel(turnPower);
    }
}

//<Inspectable>

std::vector<ItemDescription> Hero::getDebugDescription() {
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

std::string Hero::getDebugName() {
    return "Hero";
}


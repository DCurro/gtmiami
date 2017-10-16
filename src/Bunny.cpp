#include "Bunny.hpp"

#include "PlayContext.hpp"
#include "GameContext.hpp"
#include "Animation.hpp"
#include "ResourceManager.hpp"
#include "DriverMind.hpp"
#include "Mind.hpp"
#include "Camera.hpp"
#include "RandomIdentifierProvider.hpp"

Bunny::Bunny() {
    mWalkingAnimation = NULL;
    mIdleAnimation = NULL;
    mDeadAnimation = NULL;
}

b2Body* Bunny::getBody() {
    return mBody;
}

Bunny::~Bunny() {
}

void Bunny::setup(PlayContext* playContext, int persistentIdNumber, Mind* mind, int floor) {
    Animal::setup(playContext, persistentIdNumber, mind, floor);
    
    mIsPressingGasOrBreak = false;
    mIsSteering = false;
    
    mWalkingAnimation = new Animation(true,
                                      10.0f,
                                      mPlayContext->getGameContext()->getResourceManager()->getImage(ResourceManager::ImageResources::bunnyWalkCycle_png));
    
    mIdleAnimation = new Animation(false,
                                   0.0f,
                                   mPlayContext->getGameContext()->getResourceManager()->getImage(ResourceManager::ImageResources::bunnyIdleCycle_png));
    
    mDeadAnimation = new Animation(false,
                                   0.0f,
                                   mPlayContext->getGameContext()->getResourceManager()->getImage(ResourceManager::ImageResources::bunnyDeadCycle_png));
}

void Bunny::prepareForReuse() {
    Animal::prepareForReuse();
    
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

void Bunny::update(float timeStep){
    Animal::update(timeStep);
    
    if(isValid(mAutomobileDriving)){
        mAngle = mAutomobileDriving->getAngle();
        
        mIsPressingGasOrBreak = false;
        mIsSteering = false;
        return;
    }
}

#pragma mark - Animal abstract methods

float Bunny::motionSpeed() {
    return 3.0f;
}

float Bunny::maxHealthPoints() {
    return 1.0f;
}

//animations in different states
Animation* Bunny::idleAnimation() {
    return mIdleAnimation;
}

Animation* Bunny::walkAnimation() {
    return mWalkingAnimation;
}

Animation* Bunny::deadAnimation() {
    return mDeadAnimation;
}

void Bunny::hearWeaponSoundAtLocation(b2Vec2 location) {
    this->mMind->hearSoundAtLocation(location);
}

#pragma mark - Driver abstract methods

void Bunny::drive() {
    if(!isValid(mAutomobileDriving)){
        return;
    }
    
    DriverMind* driverMind = dynamic_cast<DriverMind*>(mMind);
    if(driverMind) {
        driverMind->drive();
    }
}

void Bunny::willBeginDriving() {
    this->getBodyFixture()->SetSensor(true);
    mIsDriving = true;
    
    mPlayContext->getCamera()->setZoomLevel(2.0f);
}

void Bunny::willEndDriving() {
    this->getBodyFixture()->SetSensor(false);
    mIsDriving = false;
    
    mPlayContext->getCamera()->setZoomLevel(1.0f);
}

void Bunny::requestDropOffDriver() {
    if(mAutomobileDriving) {
        mAutomobileDriving->dropOffDriver();
    }
}

void Bunny::requestApplyBreak() {
    if(mAutomobileDriving) {
        if(!mIsSteering) {
            mAutomobileDriving->applyBreak();
            mIsPressingGasOrBreak = true;
        }
    }
}

void Bunny::requestApplyGas(float gasPower) {
    if(mAutomobileDriving) {
        if(!mIsSteering) {
            mAutomobileDriving->applyGas(gasPower);
            mIsPressingGasOrBreak = true;
        }
    }
}

void Bunny::requestTurnWheel(float turnPower) {
    if(mAutomobileDriving) {
        if(!mIsPressingGasOrBreak) {
            mAutomobileDriving->turnWheel(turnPower);
            mIsSteering = true;
        }
    }
}

#pragma mark - <Inspectable>

std::vector<ItemDescription> Bunny::getDebugDescription() {
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

std::string Bunny::getDebugName() {
    return "Bunny";
}

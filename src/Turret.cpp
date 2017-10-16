#include "Turret.hpp"

#include <sstream>

#include "common.hpp"
#include "InputHandler.hpp"
#include "PlayContext.hpp"
#include "Factory.hpp"
#include "GameContext.hpp"
#include "ResourceManager.hpp"
#include "EntityManager.hpp"
#include "Automobile.hpp"
#include "Mind.hpp"
#include "RandomIdentifierProvider.hpp"

Turret::Turret() : Person() {
    mIdleAnimation = NULL;
    mDeadAnimation = NULL;
}


Turret::~Turret() {
    this->prepareForReuse();
}

void Turret::setup(PlayContext* playContext, int persistentIdNumber, Mind* mind, int floor) {
    Person::setup(playContext, persistentIdNumber, mind, floor);
    
    mIdleAnimation = new Animation(false,
                                   0.0f,
                                   mPlayContext->getGameContext()->getResourceManager()->getImage(ResourceManager::ImageResources::turretTop_png));
    
    mDeadAnimation = new Animation(false,
                                   0.0f,
                                   mPlayContext->getGameContext()->getResourceManager()->getImage(ResourceManager::ImageResources::turretTop_png));
}

void Turret::prepareForReuse() {
    Person::prepareForReuse();
    
    if(mIdleAnimation) {
        delete mIdleAnimation;
    }
    mIdleAnimation = NULL;
    
    if(mDeadAnimation) {
        delete mDeadAnimation;
    }
    mDeadAnimation = NULL;
}

void Turret::update(float timeStep){
    Person::update(timeStep);
    
    if(!mCurrentWeapon) {
        this->pickUpClosestWeapon();
    }
}

void Turret::setTargetEntity(PooledPlayEntity* targetEntity){
    if(mMind) {
        mMind->setTargetEntity(targetEntity);
    }
}

// PlayEntity

Animation* Turret::newBulletStruckAnimation() {
    sf::Image* image = mPlayContext->getGameContext()->getResourceManager()->getImage(ResourceManager::ImageResources::sparkBulletStrikeAnimation_png);;
    return new Animation(false, 24, image);
}

// Animal

float Turret::motionSpeed() {
    return 10.0f;
}

float Turret::maxHealthPoints() {
    return 3.0f;
}

Animation* Turret::idleAnimation() {
    return mIdleAnimation;
}

Animation* Turret::walkAnimation() {
    return mIdleAnimation;
}

Animation* Turret::deadAnimation() {
    return mDeadAnimation;
}

void Turret::hearWeaponSoundAtLocation(b2Vec2 location) {
    UNUSED(location);
}

#pragma mark - <Inspectable>

std::vector<ItemDescription> Turret::getDebugDescription() {
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

std::string Turret::getDebugName() {
    return "Turret";
}



#include "Zombie.hpp"
#include "PlayContext.hpp"
#include "ResourceManager.hpp"
#include "GameContext.hpp"
#include "Mind.hpp"
#include "Animation.hpp"
#include "RandomIdentifierProvider.hpp"

Zombie::Zombie() : Person() {
    mWalkingAnimation = NULL;
    mIdleAnimation = NULL;
    mDeadAnimation = NULL;
}


Zombie::~Zombie() {
    this->prepareForReuse();
}

void Zombie::setup(PlayContext* playContext, int persistentIdNumber, Mind* mind, int floor) {
    Person::setup(playContext, persistentIdNumber, mind, floor);
    
    mWalkingAnimation = new Animation(true,
                                      24.0f,
                                      mPlayContext->getGameContext()->getResourceManager()->getImage(ResourceManager::ImageResources::zombieWalkCycle_png));
    
    mIdleAnimation = new Animation(false,
                                   0.0f,
                                   mPlayContext->getGameContext()->getResourceManager()->getImage(ResourceManager::ImageResources::zombieIdleCycle_png));
    
    mDeadAnimation = new Animation(false,
                                   0.0f,
                                   mPlayContext->getGameContext()->getResourceManager()->getImage(ResourceManager::ImageResources::turretTop_png));
}

void Zombie::prepareForReuse() {
    Person::prepareForReuse();
    
    if (mWalkingAnimation) {
        delete mWalkingAnimation;
        mWalkingAnimation = NULL;
    }
    
    if (mIdleAnimation) {
        delete mIdleAnimation;
        mIdleAnimation = NULL;
    }
    
    if (mDeadAnimation) {
        delete mDeadAnimation;
        mDeadAnimation = NULL;
    }
}

void Zombie::update(float timeStep){
    Person::update(timeStep);
    
    //    if(!mCurrentWeapon) {
    //        this->pickUpClosestWeapon();
    //    }
}

void Zombie::setTargetEntity(PooledPlayEntity* targetEntity){
    if(mMind) {
        mMind->setTargetEntity(targetEntity);
    }
}

#pragma mark - PlayEntity Abstract Methods

Animation*Zombie::newBulletStruckAnimation() {
    sf::Image* image = mPlayContext->getGameContext()->getResourceManager()->getImage(ResourceManager::ImageResources::sparkBulletStrikeAnimation_png);;
    return new Animation(false, 24, image);
}

#pragma mark - Animal Abstract Methods

float Zombie::motionSpeed() {
    return 6.0f;
}

float Zombie::maxHealthPoints() {
    return 3.0f;
}

Animation*Zombie::idleAnimation() {
    return mIdleAnimation;
}

Animation*Zombie::walkAnimation() {
    return mWalkingAnimation;
}

Animation*Zombie::deadAnimation() {
    return mDeadAnimation;
}

void Zombie::hearWeaponSoundAtLocation(b2Vec2 location) {
    this->mMind->hearSoundAtLocation(location);
}

#pragma mark - <Inspectable>

std::vector<ItemDescription> Zombie::getDebugDescription() {
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

std::string Zombie::getDebugName() {
    return "Zombie";
}

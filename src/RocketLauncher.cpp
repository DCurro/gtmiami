#include "RocketLauncher.hpp"

#include "PlayContext.hpp"
#include "GameContext.hpp"
#include "Mind.hpp"
#include "ResourceManager.hpp"
#include "Animation.hpp"
#include "Factory.hpp"
#include "RandomIdentifierProvider.hpp"

//Note upon impact, explodes, this class would be great for a grenade launcher

RocketLauncher::RocketLauncher() {
    mIdleAnimation = NULL;
}

RocketLauncher::~RocketLauncher() {
}

void RocketLauncher::setup(PlayContext* playContext, int persistentReferenceId, int floor) {
    ProjectileGun::setup(playContext, persistentReferenceId, floor);
    
    ResourceManager* resourceManager = mPlayContext->getGameContext()->getResourceManager();
    mIdleAnimation = new Animation(false, 0, resourceManager->getImage(ResourceManager::ImageResources::rocketLauncher_png));
}

void RocketLauncher::prepareForReuse() {
    ProjectileGun::prepareForReuse();
    
    if(mIdleAnimation) {
        delete mIdleAnimation;
    }
}

// PlayEntity

Animation* RocketLauncher::newBulletStruckAnimation() {
    sf::Image* image = mPlayContext->getGameContext()->getResourceManager()->getImage(ResourceManager::ImageResources::sparkBulletStrikeAnimation_png);
    return new Animation(false, 24, image);
}

void RocketLauncher::struckByBulletConsequences(float bullerCalibre) {
    UNUSED(bullerCalibre);
}

void RocketLauncher::struckByBlastConsequences(b2Vec2 blastForce, float damage) {
    UNUSED(blastForce);
    UNUSED(damage);
}

// Weapon

bool RocketLauncher::strikeAtEndOfUse() {
    return true;
}


float RocketLauncher::soundRadius() {
    return 0.0; // TODO: define radius
}

// Gun

int RocketLauncher::maxAmmoCount() {
    return 30;
}

float RocketLauncher::cooldownTimeInSeconds() {
    return 3.0f;
}

float RocketLauncher::rangeInMeters() {
    return 3.0f;
}

Animation* RocketLauncher::idleAnimation() {
    return mIdleAnimation;
}

bool RocketLauncher::isAutomaticWeapon() {
    return false;
}

ResourceManager::SoundResources RocketLauncher::actionSoundEffect() {
    return ResourceManager::SoundResources::rocketBlast_wav;
}

void RocketLauncher::strike(float angle) {
    ProjectileGun::strike(angle);
    
    if(mAmmoCount <= 0) {
        this->mIsStillPartOfGame = false;
    }
}

//Projectile Gun

PooledPlayEntity* RocketLauncher::newProjectile(int floor, b2Vec2 position, float angle) {
    return mPlayContext->getFactory()->createRocketProjectile(mPlayContext, floor, position, angle);
}

float RocketLauncher::launchVelocity() {
    return 7.0f;
}

// <Inspectable>

std::vector<ItemDescription> RocketLauncher::getDebugDescription() {
    std::vector<ItemDescription> itemDescriptionList;
    
    std::stringstream ss;
    if( mPersistentIdNumber == NO_PERSISTENCE_ID) {
        ss << "persistence id: " << "NON-PERSISTENT OBJECT" << std::endl;
    } else {
        ss << "persistence id: " << mPersistentIdNumber << std::endl;
    }
    ItemDescription idDescription(ss.str(), NULL, ItemDescription::UsageType::NO_USAGE);
    itemDescriptionList.push_back(idDescription);
    
    ss.clear();
    ss << "current floor: " << mCurrentFloor << std::endl;
    ItemDescription floorDescription(ss.str(), NULL, ItemDescription::UsageType::NO_USAGE);
    itemDescriptionList.push_back(floorDescription);
    
    return itemDescriptionList;
}

std::string RocketLauncher::getDebugName() {
    return "Rocket Launcher";
}


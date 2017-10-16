#include "Grenade.hpp"

#include "PlayContext.hpp"
#include "GameContext.hpp"
#include "Mind.hpp"
#include "Animation.hpp"
#include "ResourceManager.hpp"
#include "Factory.hpp"
#include "RandomIdentifierProvider.hpp"

Grenade::Grenade() {
    mIdleAnimation = NULL;
}

Grenade::~Grenade() {
    
}

void Grenade::setup(PlayContext* playContext, int persistentReferenceId, int floor) {
    AuxiliaryWeapon::setup(playContext, persistentReferenceId, floor);
    
    ResourceManager* resourceManager = mPlayContext->getGameContext()->getResourceManager();
    mIdleAnimation = new Animation(false, 0, resourceManager->getImage(ResourceManager::ImageResources::grenadeStash_png));
}

void Grenade::prepareForReuse() {
    AuxiliaryWeapon::prepareForReuse();
    
    if(mIdleAnimation) {
        delete mIdleAnimation;
    }
}

// Weapon

bool Grenade::strikeAtEndOfUse() {
    return true;
}

float Grenade::soundRadius() {
    return 0.0; // TODO: maybe have a tossing sound?
}


// PlayEntity

Animation* Grenade::newBulletStruckAnimation() {
    sf::Image* image = mPlayContext->getGameContext()->getResourceManager()->getImage(ResourceManager::ImageResources::sparkBulletStrikeAnimation_png);;
    return new Animation(false, 24, image);
}

void Grenade::struckByBulletConsequences(float bulletCalibre) {
    UNUSED(bulletCalibre);
}

void Grenade::struckByBlastConsequences(b2Vec2 blastForce, float damage) {
    UNUSED(blastForce);
    UNUSED(damage);
}

// Gun

int Grenade::maxAmmoCount() {
    return 5;
}

float Grenade::cooldownTimeInSeconds() {
    return 0.0f;
}

float Grenade::rangeInMeters() {
    return 7.0f;
}

Animation* Grenade::idleAnimation() {
    return mIdleAnimation;
}

ResourceManager::SoundResources Grenade::actionSoundEffect() {
    return ResourceManager::SoundResources::no_sound;
}

void Grenade::strike(float angle) {
    AuxiliaryWeapon::strike(angle);
    
    if(mAmmoCount <= 0) {
        this->mIsStillPartOfGame = false;
    }
}

// Projectile Gun

PooledPlayEntity* Grenade::newProjectile(int floor, b2Vec2 position, float angle) {
    return mPlayContext->getFactory()->createGrenadeProjectile(mPlayContext, floor, position, angle);
}

float Grenade::launchVelocity() {
    return 4.0f;
}

// <Inspectable>

std::vector<ItemDescription> Grenade::getDebugDescription() {
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

std::string Grenade::getDebugName() {
    return "Grenade";
}

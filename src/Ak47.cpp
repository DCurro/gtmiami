#include "Ak47.hpp"

#include "PlayContext.hpp"
#include "GameContext.hpp"
#include "ResourceManager.hpp"
#include "Animation.hpp"
#include "RandomIdentifierProvider.hpp"

Ak47::Ak47() {
    mAnimation = NULL;
}

Ak47::~Ak47() {
}

void Ak47::setup(PlayContext* playContext, int persistentIdNumber, int floor) {
    BulletGun::setup(playContext, persistentIdNumber, floor);
    
    ResourceManager* resourceManager = mPlayContext->getGameContext()->getResourceManager();
    mAnimation = new Animation(false, 0, resourceManager->getImage(ResourceManager::ImageResources::ak47_png));
}

void Ak47::prepareForReuse() {
    BulletGun::prepareForReuse();
    
    if(mAnimation) {
        delete mAnimation;
        mAnimation = NULL;
    }
}

// Weapon

bool Ak47::strikeAtEndOfUse() {
    return false;
}

float Ak47::soundRadius() {
    return 5.0; // TODO: define radius
}

// Gun

int Ak47::maxAmmoCount() {
    return 40;
}

float Ak47::cooldownTimeInSeconds() {
    return 0.1f;
}

float Ak47::rangeInMeters() {
    return 7.0f;
}

Animation* Ak47::idleAnimation() {
    return mAnimation;
}

bool Ak47::isAutomaticWeapon() {
    return true;
}

ResourceManager::SoundResources Ak47::actionSoundEffect() {
    return ResourceManager::SoundResources::gunBlast_wav;
}

// BulletCalibre

float Ak47::bulletCalibre() {
    return 1.5f;
}

// <Inspectable>

std::vector<ItemDescription> Ak47::getDebugDescription() {
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

std::string Ak47::getDebugName() {
    return "Ak47";
}

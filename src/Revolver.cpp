#include "Revolver.hpp"

#include "PlayContext.hpp"
#include "GameContext.hpp"
#include "ResourceManager.hpp"
#include "Animation.hpp"
#include "RandomIdentifierProvider.hpp"

Revolver::Revolver() {
    mAnimation = NULL;
}

Revolver::~Revolver() {
}

void Revolver::setup(PlayContext* playContext, int persistentIdNumber, int floor) {
    BulletGun::setup(playContext, persistentIdNumber, floor);
    
    ResourceManager* resourceManager = mPlayContext->getGameContext()->getResourceManager();
    mAnimation = new Animation(false, 0, resourceManager->getImage(ResourceManager::ImageResources::floorRevolver_png));
}

void Revolver::prepareForReuse() {
    BulletGun::prepareForReuse();
    
    if(mAnimation) {
        delete mAnimation;
        mAnimation = NULL;
    }
}

// Weapon

bool Revolver::strikeAtEndOfUse() {
    return true;
}

float Revolver::soundRadius() {
    return 5.0;  // TODO: define radius
}

// Gun

int Revolver::maxAmmoCount() {
    return 8;
}

float Revolver::cooldownTimeInSeconds() {
    return 0.0f;
}

float Revolver::rangeInMeters() {
    return 7.0f;
}

Animation* Revolver::idleAnimation() {
    return mAnimation;
}

bool Revolver::isAutomaticWeapon() {
    return false;
}

ResourceManager::SoundResources Revolver::actionSoundEffect() {
    return ResourceManager::SoundResources::gunBlast_wav;
}

// BulletGun

float Revolver::bulletCalibre() {
    return 1.0f;
}

// <Inspectable>

std::vector<ItemDescription> Revolver::getDebugDescription() {
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

std::string Revolver::getDebugName() {
    return "Revolver";
}

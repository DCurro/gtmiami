/*
 * KungFuPrimalWeapon.cpp
 *
 *  Created on: Dec 13, 2014
 *      Author: DCurro
 */

#include "KungFuPrimalWeapon.hpp"
#include "RandomIdentifierProvider.hpp"

KungFuPrimalWeapon::KungFuPrimalWeapon() {
}

KungFuPrimalWeapon::~KungFuPrimalWeapon() {
}

Animation* KungFuPrimalWeapon::idleAnimation() {
    return NULL;
}

float KungFuPrimalWeapon::cooldownTimeInSeconds() {
    return 0.0f;
}

ResourceManager::SoundResources KungFuPrimalWeapon::actionSoundEffect() {
    return ResourceManager::SoundResources::punch_wav;
}

void KungFuPrimalWeapon::strikeWithAngle(float angle) {
    UNUSED(angle);
}

bool KungFuPrimalWeapon::strikeAtEndOfUse() {
    return true;
}

// <Inspectable>

std::vector<ItemDescription> KungFuPrimalWeapon::getDebugDescription() {
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

std::string KungFuPrimalWeapon::getDebugName() {
    return "KungFuPrimalWeapon";
}

#include "TowTruck.hpp"
#include "GameContext.hpp"

#include <sstream>
#include <set>

#include "PlayEntity.hpp"
#include "PlayContext.hpp"
#include "InputHandler.hpp"
#include "ResourceManager.hpp"
#include "DriverBody.hpp"
#include "RandomIdentifierProvider.hpp"

TowTruck::TowTruck() {
    mIdleAnimation = NULL;
}

TowTruck::~TowTruck() {
}

void TowTruck::setup(PlayContext* playContext, int persistentIdNumber, int floor, b2Body* body, int teamID) {
    Automobile::setup(playContext, persistentIdNumber, floor);
    
    this->mBody = body;
    this->mTeamID = teamID;
    
    sf::Image* idleImage = mPlayContext->getGameContext()->getResourceManager()->getImage(ResourceManager::ImageResources::towTruck_png);
    mIdleAnimation = new Animation(false, 0, idleImage);
}

void TowTruck::prepareForReuse() {
    Automobile::prepareForReuse();
    
    if(mIdleAnimation) {
        delete mIdleAnimation;
    }
    mIdleAnimation = NULL;
}

//<Automobile>

float TowTruck::maxForwardVelocity() {
    return 100.0f;
}

float TowTruck::maxReverseVelocity() {
    return 1.0f;
}

float TowTruck::maxForwardForce() {
    return 4000.0f;
}

float TowTruck::maxBreakingForce() {
    return 10000.0f;
    
}

float TowTruck::turnCoefficient() {
    return 1.0f;
}

float TowTruck::maxImpulseBeforeDrifting() {
    return 300.0f;
}

float TowTruck::dragAndTireFrictionWhileInDriveCoefficient() {
    return 2;
}

float TowTruck::dragAndTireFrictionWhileParkedCoefficient() {
    return 10000;
}

float TowTruck::angularImpulseWhileInDrive() {
    return 0.1f;
}

float TowTruck::angularImpulseWhileParked() {
    return 2.0f;
}

Animation* TowTruck::idleAnimation() {
    return mIdleAnimation;
}

//<Inspectable>

std::vector<ItemDescription> TowTruck::getDebugDescription() {
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

std::string TowTruck::getDebugName() {
    return "Truck";
}

#include "GrenadeProjectile.hpp"

#include "PlayContext.hpp"
#include "GameContext.hpp"
#include "ResourceManager.hpp"
#include "Animation.hpp"
#include "Factory.hpp"
#include "EntityManager.hpp"
#include "RandomIdentifierProvider.hpp"

GrenadeProjectile::GrenadeProjectile() {
    mIdleAnimation = NULL;
}

GrenadeProjectile::~GrenadeProjectile() {
}

void GrenadeProjectile::setup(PlayContext* playContext, int floor) {
    Projectile::setup(playContext, floor);
    
    ResourceManager* resourceManager = mPlayContext->getGameContext()->getResourceManager();
    mIdleAnimation = new Animation(true, 4, resourceManager->getImage(ResourceManager::ImageResources::grenade_png));
    
    mTimeAccumulatorForDetonation = 0.0f;
}

void GrenadeProjectile::prepareForReuse() {
    Projectile::prepareForReuse();
    
    if(this->mIdleAnimation) {
        delete this->mIdleAnimation;
    }
}

void GrenadeProjectile::update(float timestep) {
    Projectile::update(timestep);
    
    mTimeAccumulatorForDetonation += timestep;
    if(mTimeAccumulatorForDetonation > timeUntilDetontation) {
        Explosion* explosion = mPlayContext->getFactory()->createExplosion(mPlayContext, this->getCurrentFloor(), this->getBody()->GetPosition(), this->getBody()->GetAngle(), 2.0f, 10.0f);
        mPlayContext->getEntityManager()->addEntity(explosion);
        
        this->mIsStillPartOfGame = false;
    }
    
    float angle = angleInRadiansFromVector(this->getBody()->GetLinearVelocity());
    this->getBody()->SetTransform(this->getBody()->GetPosition(), angle);
}

// PlayEntity

Animation* GrenadeProjectile::newBulletStruckAnimation() {
    sf::Image* image = mPlayContext->getGameContext()->getResourceManager()->getImage(ResourceManager::ImageResources::sparkBulletStrikeAnimation_png);;
    return new Animation(false, 24, image);
}

void GrenadeProjectile::struckByBulletConsequences(float bulletCalibre) {
    mTimeAccumulatorForDetonation += bulletCalibre;
}

void GrenadeProjectile::struckByBlastConsequences(b2Vec2 blastForce, float damage) {
    UNUSED(blastForce);
    
    mTimeAccumulatorForDetonation += damage;
}

// Projectile

Animation* GrenadeProjectile::idleAnimation() {
    return mIdleAnimation;
}

// <Inspectable>

std::vector<ItemDescription> GrenadeProjectile::getDebugDescription() {
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

std::string GrenadeProjectile::getDebugName() {
    return "GrenadeProjectile";
}

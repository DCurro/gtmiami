#include "Explosion.hpp"

#include "PlayContext.hpp"
#include "GameContext.hpp"
#include "ResourceManager.hpp"
#include "AudioManager.hpp"
#include "Animation.hpp"
#include "Camera.hpp"
#include "ExplosionCircleCaster.hpp"
#include "CircleAnimalFinderQueryCallback.hpp"
#include "RandomIdentifierProvider.hpp"

Explosion::Explosion() {
    mRadius = 0.0f;
    mBaseDamage = 0.0f;
    
    mAnimation = NULL;
}

Explosion::~Explosion() {
}


void Explosion::setup(PlayContext* playContext, int floor, float radius, float baseDamage) {
    PooledPlayEntity::setup(playContext, floor);
    
    mRadius = radius;
    mBaseDamage = baseDamage;
    
    performOnce = 0;
    
    sf::Image* image = playContext->getGameContext()->getResourceManager()->getImage(ResourceManager::ImageResources::explosion_png);
    mAnimation = new Animation(false, 16, image);
    
    float imageRadius = mAnimation->getCurrentFrame()->getSize().y;
    float worldRadiusInScreenSpace = Camera::worldToScreen(sf::Vector2f(radius,radius)).y;
    
    float finalScale = (worldRadiusInScreenSpace/imageRadius) * 2.0f;
    
    mSprite.setScale(sf::Vector2f(finalScale,finalScale));
    
    mPlayContext->getGameContext()->getAudioManager()->play(ResourceManager::SoundResources::explosion_wav);
}

void Explosion::prepareForReuse() {
    PooledPlayEntity::prepareForReuse();
    
    mRadius = 0.0f;
    
    if(mAnimation) {
        delete mAnimation;
    }
    mAnimation = NULL;
}

void Explosion::update(float timeStep) {
    PooledPlayEntity::update(timeStep);
    
    //only happens once
    if(performOnce++ == 0){
        this->getBody()->SetTransform(this->getBody()->GetPosition(), (std::rand() % 360) * DEGREES_TO_RADIANS);
        
        this->damageNearbyEnemies();
        this->informNearbyAnimalsOfSounds();
    }
    
    if(mAnimation) {
        mAnimation->update(timeStep);
        mSprite.setTexture(*mAnimation->getCurrentFrame(), true);
        
        if(mAnimation->hasCompletedAnimation()) {
            this->mIsStillPartOfGame = false;
        }
    }
}

// PlayEntity

Animation* Explosion::newBulletStruckAnimation() {
    return NULL;
}

void Explosion::struckByBulletConsequences(float bulletCalibre) {
    UNUSED(bulletCalibre);
}

void Explosion::struckByBlastConsequences(b2Vec2 blastForce, float damage) {
    UNUSED(blastForce);
    UNUSED(damage);
}

//<Inspectable>

std::vector<ItemDescription> Explosion::getDebugDescription() {
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

std::string Explosion::getDebugName() {
    return "Explosion";
}

#pragma mark - Private Methods

void Explosion::damageNearbyEnemies()
{
    ExplosionCircleCaster explosionCast(mPlayContext,mCurrentFloor,mRadius,this->getBody()->GetPosition());
    explosionCast.cast();
    for(auto pair : explosionCast.getEntitiesAffectedByExplosion()) {
        //TODO: calculate blast force
        pair.first->strikeWithBlastForce(b2Vec2(0,0), mBaseDamage*pair.second);
        std::cout << "EXPLOSION hit " << pair.first->getDebugName() << " with distance from blast " << pair.second << std::endl;
    }
}

void Explosion::informNearbyAnimalsOfSounds()
{
    CircleAnimalFinderQueryCallback animalFinderCallback(mPlayContext, mCurrentFloor, mRadius*3, this->getBody()->GetPosition());
    std::vector<Animal *> animalsThatHearSound = animalFinderCallback.queryWorldAndGetAnimals();
    
    for (Animal* animal : animalsThatHearSound) {
        animal->hearWeaponSoundAtLocation(this->mBody->GetPosition());
        std::cout << animal->getDebugName() << " heard an explosion sound" << std::endl;
    }
}

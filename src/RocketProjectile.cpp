#include "RocketProjectile.hpp"

#include "PlayContext.hpp"
#include "GameContext.hpp"
#include "ResourceManager.hpp"
#include "Animation.hpp"
#include "Factory.hpp"
#include "EntityManager.hpp"

RocketProjectile::RocketProjectile() {
	mIdleAnimation = NULL;
}

RocketProjectile::~RocketProjectile() {
}

void RocketProjectile::setup(PlayContext* playContext, int floor) {
	Projectile::setup(playContext, floor);

	ResourceManager* resourceManager = mPlayContext->getGameContext()->getResourceManager();
	mIdleAnimation = new Animation(false, 4, resourceManager->getImage(ResourceManager::ImageResources::rocket_png));

	mTimeAccumulatorForDetonation = 0.0f;

	collision = false;
}

void RocketProjectile::prepareForReuse() {
	Projectile::prepareForReuse();

	if(this->mIdleAnimation) {
		delete this->mIdleAnimation;
	}
}

void RocketProjectile::update(float timestep) {
	Projectile::update(timestep);

	if(collision == false) {
		mTimeAccumulatorForDetonation += timestep;
		if(mTimeAccumulatorForDetonation > timeUntilDetontation) {
			Explosion* explosion = mPlayContext->getFactory()->createExplosion(mPlayContext, this->getCurrentFloor(), this->getBody()->GetPosition(), this->getBody()->GetAngle(), 5.0f, 20.0f);
			mPlayContext->getEntityManager()->addEntity(explosion);

			this->mIsStillPartOfGame = false;
		}
		float angle = angleInRadiansFromVector(this->getBody()->GetLinearVelocity());
		this->getBody()->SetTransform(this->getBody()->GetPosition(), angle);

	} else if (collision == true) {
		mTimeAccumulatorForDetonation += timestep;
		if(mTimeAccumulatorForDetonation > explodeOnContact) {
			Explosion* explosion = mPlayContext->getFactory()->createExplosion(mPlayContext, this->getCurrentFloor(), this->getBody()->GetPosition(), this->getBody()->GetAngle(), 5.0f, 20.0f);
			mPlayContext->getEntityManager()->addEntity(explosion);

			this->mIsStillPartOfGame = false;
		}
		float angle = angleInRadiansFromVector(this->getBody()->GetLinearVelocity());
		this->getBody()->SetTransform(this->getBody()->GetPosition(), angle);

		collision = false;
	}



}

//CollisionListner
void RocketProjectile::rocketCollisionExplosion() {
	collision = true;
	RocketProjectile();
}

//PlayEntity

Animation* RocketProjectile::newBulletStruckAnimation() {
	sf::Image* image = mPlayContext->getGameContext()->getResourceManager()->getImage(ResourceManager::ImageResources::sparkBulletStrikeAnimation_png);
	return new Animation(false, 24, image);
}

void RocketProjectile::struckByBulletConsequences(float bulletCalibre) {
	mTimeAccumulatorForDetonation += bulletCalibre;
}

void RocketProjectile::struckByBlastConsequences(b2Vec2 blastForce, float damage) {
	UNUSED(blastForce);

	mTimeAccumulatorForDetonation += damage;
}

//Projectile

Animation* RocketProjectile::idleAnimation() {
	return mIdleAnimation;
}

//<Inspectable>

std::vector<ItemDescription> RocketProjectile::getDebugDescription() {
	std::vector<ItemDescription> itemDescriptionList;

	std::stringstream ss;
	ss <<"unique id: " << mPersistentIdNumber << std::endl;
	ItemDescription idDescription(ss.str(), NULL, ItemDescription::UsageType::NO_USAGE);
	itemDescriptionList.push_back(idDescription);

	ss.clear();
	ss << "current floor: " << mCurrentFloor << std::endl;
	ItemDescription floorDescription(ss.str(), NULL, ItemDescription::UsageType::NO_USAGE);
	itemDescriptionList.push_back(floorDescription);

	return itemDescriptionList;
}

std::string RocketProjectile::getDebugName() {
	return "RocketProjectile";
}

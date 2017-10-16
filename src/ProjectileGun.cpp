#include "ProjectileGun.hpp"

#include "PlayContext.hpp"
#include "EntityManager.hpp"

ProjectileGun::ProjectileGun() {
}

ProjectileGun::~ProjectileGun() {
}

void ProjectileGun::setup(PlayContext* playContext, int persistentReferenceId, int floor) {
	Gun::setup(playContext, persistentReferenceId, floor);
}

void ProjectileGun::strikeWithAngle(float angle) {
	float xOffset = std::cos(angle*DEGREES_TO_RADIANS) * 0.5f; //TODO: don't use 0.5; user the body fixture's radius (or longest side)
	float yOffset = std::sin(angle*DEGREES_TO_RADIANS) * 0.5f; //TODO: don't use 0.5; user the body fixture's radius (or longest side)
	PooledPlayEntity* projectile = this->newProjectile(this->getCurrentFloor(), this->getBody()->GetPosition() + b2Vec2(xOffset,yOffset), angle);
	if(projectile != NULL) {
		mPlayContext->getEntityManager()->addEntity(projectile);
		float inertiaX = std::cos(angle*DEGREES_TO_RADIANS) * this->launchVelocity() * projectile->getBody()->GetMass();
		float inertiaY = std::sin(angle*DEGREES_TO_RADIANS) * this->launchVelocity() * projectile->getBody()->GetMass();
		projectile->getBody()->ApplyLinearImpulse(b2Vec2(inertiaX,inertiaY), projectile->getBody()->GetWorldCenter(), true);
	}
}

/*
 * AuxiliaryWeapon.cpp
 *
 *  Created on: Dec 9, 2014
 *      Author: DCurro
 */

#include "AuxiliaryWeapon.hpp"

#include "PlayContext.hpp"
#include "EntityManager.hpp"

AuxiliaryWeapon::AuxiliaryWeapon() {
}

AuxiliaryWeapon::~AuxiliaryWeapon() {
}

void AuxiliaryWeapon::setup(PlayContext* playContext, int persistentReferenceId, int floor) {
	Weapon::setup(playContext, persistentReferenceId, floor);

	mAmmoCount = this->maxAmmoCount();
}

void AuxiliaryWeapon::strike(float angle) {
	if(mAmmoCount <= 0) {
		return;
	}

	if(mAccumulatedCooldownTime < this->cooldownTimeInSeconds()) {
		return;
	}

	if(mCanAttackWithWeapon == false) {
		return;
	}
	mCanAttackWithWeapon = false;

	mAmmoCount = mAmmoCount-1;

	Weapon::strike(angle);
}

void AuxiliaryWeapon::strikeWithAngle(float angle) {
	float xOffset = std::cos(angle*DEGREES_TO_RADIANS) * 0.5f; //TODO: don't use 0.5; use the body fixture's radius (or longest side)
	float yOffset = std::sin(angle*DEGREES_TO_RADIANS) * 0.5f; //TODO: don't use 0.5; use the body fixture's radius (or longest side)
	PooledPlayEntity* projectile = this->newProjectile(this->getCurrentFloor(), this->getBody()->GetPosition() + b2Vec2(xOffset,yOffset), angle);
	if(projectile != NULL) {
		mPlayContext->getEntityManager()->addEntity(projectile);
		float inertiaX = std::cos(angle*DEGREES_TO_RADIANS) * this->launchVelocity() * projectile->getBody()->GetMass();
		float inertiaY = std::sin(angle*DEGREES_TO_RADIANS) * this->launchVelocity() * projectile->getBody()->GetMass();
		projectile->getBody()->ApplyLinearImpulse(b2Vec2(inertiaX,inertiaY), projectile->getBody()->GetWorldCenter(), true);
	}
}

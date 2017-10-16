/*
 * AuxiliaryWeapon.hpp
 *
 *  Created on: Dec 9, 2014
 *      Author: DCurro
 */

#ifndef AUXILIARYWEAPON_HPP_
#define AUXILIARYWEAPON_HPP_

#include "Weapon.hpp"

class AuxiliaryWeapon: public Weapon {
protected:
	// return a pointer to a new instance of a projectile (do not delete the projectile.  Memory management will be handled automatically.)
	virtual PooledPlayEntity* newProjectile(int floor, b2Vec2 position, float angle) = 0;
	virtual float launchVelocity() = 0;

	// Gun
	virtual void strikeWithAngle(float angle) override;

	virtual int maxAmmoCount() = 0;

	GETTER(int, AmmoCount);


public:
	AuxiliaryWeapon();
	virtual ~AuxiliaryWeapon();

	using Weapon::setup;
	virtual void setup(PlayContext* playContext, int persistentReferenceId, int floor);

	void strike(float angle);
};

#endif /* AUXILIARYWEAPON_HPP_ */

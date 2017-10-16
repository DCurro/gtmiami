#ifndef PROJECTILEGUN_HPP_
#define PROJECTILEGUN_HPP_

#include "Gun.hpp"

class ProjectileGun: public Gun {
protected:
	// return a pointer to a new instance of a projectile (do not delete the projectile.  Memory management will be handled automatically.)
	virtual PooledPlayEntity* newProjectile(int floor, b2Vec2 position, float angle) = 0;
	virtual float launchVelocity() = 0;

	// Gun
	virtual void strikeWithAngle(float angle) override;

public:
	ProjectileGun();
	virtual ~ProjectileGun();

	using Gun::setup;
	virtual void setup(PlayContext* playContext, int persistentReferenceId, int floor);
};

#endif /* PROJECTILEGUN_HPP_ */

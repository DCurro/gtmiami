#ifndef ROCKETPROJECTILE_HPP_
#define ROCKETPROJECTILE_HPP_

#include "Projectile.hpp"

class PlayContext;
class Animation;

class RocketProjectile: public Projectile {
protected:
	Animation* mIdleAnimation;

	const float timeUntilDetontation = 2.4f;
	float mTimeAccumulatorForDetonation;

	bool collision;
	const float explodeOnContact = 0.0f;

	//Projectile
	virtual Animation* idleAnimation() override;

public:

	RocketProjectile();
	virtual ~RocketProjectile();

	using Projectile::setup;
	virtual void setup(PlayContext* playContext, int floor);
	virtual void prepareForReuse();

	virtual void update(float timestep);

	//CollisionListener
	virtual void rocketCollisionExplosion();

	//PlayEntity
	//requires that the subclass return a new Animation*(..) - Dom's note
	virtual Animation* newBulletStruckAnimation() override;
	//effects a bullet would have on this entity
	virtual void struckByBulletConsequences(float bulletCalibre) override;
	void struckByBlastConsequences(b2Vec2 blastForce, float damage) override;

	//<Inspectable>
	virtual std::vector<ItemDescription> getDebugDescription() override;
	virtual std::string getDebugName() override;
};

#endif /* ROCKETPROJECTILE_HPP_ */

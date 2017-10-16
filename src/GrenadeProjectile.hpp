#ifndef GRENADEPROJECTILE_HPP_
#define GRENADEPROJECTILE_HPP_

#include "Projectile.hpp"

class PlayContext;
class Animation;

class GrenadeProjectile: public Projectile {
protected:
	Animation* mIdleAnimation;

	const float timeUntilDetontation = 3.0f;
	float mTimeAccumulatorForDetonation;

	// Projectile
	virtual Animation* idleAnimation() override;

public:
	GrenadeProjectile();
	virtual ~GrenadeProjectile();

	using Projectile::setup;
	virtual void setup(PlayContext* playContext, int floor);
	virtual void prepareForReuse();

	virtual void update(float timestep);


	// PlayEntity
	//Requires that the subclass return a new Animation*(...).  The Animation will be deleted by internal logic.  As a subclasser, do not maintain a pointer to the returned Animation*
	virtual Animation* newBulletStruckAnimation() override;
	//The affects a bullet would have on this entity
	virtual void struckByBulletConsequences(float bulletCalibre) override;
	void struckByBlastConsequences(b2Vec2 blastForce, float damage) override;

	// <Inspectable>
	virtual std::vector<ItemDescription> getDebugDescription() override;
	virtual std::string getDebugName() override;
};

#endif /* GRENADEPROJECTILE_HPP_ */
